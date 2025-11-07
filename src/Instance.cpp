#include "../include/Instance.hpp"
#include <OpenXLSX.hpp>

using namespace OpenXLSX;
using namespace std;
using namespace opthirrygated;

// Construtor já existente no seu .cpp
Instance::Instance(string path) {
    setPath(path);
    Instance::exec();
}

/*
 * loadColumn: carrega valores de uma coluna entre startCell e endCell.
 * - se endCell < 0: varre até encontrar a primeira célula vazia (ou até maxScan).
 * - tenta fazer cast para T; em caso de erro, interrompe a leitura daquela coluna.
 */
template <typename T>
vector<T> loadColumn(XLWorksheet& wks, const string& col, int startCell, int endCell = -1, int maxScan = 2000) {
    vector<T> values;
    int row = startCell;
    int limit = (endCell > 0) ? endCell : (startCell + maxScan);

    for (; row <= limit; ++row) {
        string cellRef = col + to_string(row);
        try {
            auto cell = wks.cell(XLCellReference(cellRef));
            // Se a célula estiver vazia e endCell não foi explicitado, interrompe (fim dos dados)
            if (endCell < 0) {
                if (!cell.empty() || cell.value().get<std::string>().empty()) break;
            }
            values.push_back(cell.value().get<T>());
        } catch (const std::exception& e) {
            // Se estivermos lendo até uma célula vazia (endCell < 0), interrompe silênciosamente.
            // Caso contrário, relança ou para a leitura daquela coluna.
            if (endCell < 0) break;
            else {
                cerr << "Warning loading cell " << col << row << ": " << e.what() << endl;
                break;
            }
        }
    }
    return values;
}

void Instance::exec() {
    try {
        XLDocument doc;
        doc.open(getPath());


        // Limpa pivos existentes
        clearPivots();

        // Tenta obter lista de nomes de sheets. A API do OpenXLSX oferece worksheetNames().
        auto sheetNames = doc.workbook().worksheetNames();

        for (const auto& name : sheetNames) {
            // Seleciona sheets que começam com "pivo" (case-sensitive). Ajuste se precisar.
            if (name.rfind("pivo", 0) == 0) { // starts_with("pivo")
                try {
                    auto wks = doc.workbook().worksheet(name);
                    Pivot p;

                    // Carrega os ranges conhecidos (mesma lógica do seu exemplo original).
                    // Aqui eu uso endCell fixo se for necessário, mas se a planilha tiver menos dados,
                    // o loadColumn com endCell = -1 para até a primeira célula vazia cuidará disso.
                    p.setCicle(loadColumn<float>(wks, "A", 2, 121));   // se quiser detectar dinamicamente, passe -1
                    p.setPrec(loadColumn<float>(wks, "B", 2, 121));
                    p.setEtc(loadColumn<float>(wks, "C", 2, 121));
                    p.setCad(loadColumn<float>(wks, "D", 2, 121));
                    p.setLc(loadColumn<float>(wks, "E", 2, 121));

                    // As colunas perc/cost/lamp eram lidas de outra sheet antes.
                    // Agora assumimos que, por item, estas estão na mesma sheet nas colunas C,D,E (linhas 2..12).
                    // Ajuste se o layout for diferente.
                    p.setLamp(loadColumn<float>(wks, "H", 2, 12));
                    p.setCost(loadColumn<float>(wks, "I", 2, 12));
                    p.setPerc(loadColumn<int>(wks,   "J", 2, 12));

                    // Adiciona o pivo ao vetor de pivos
                    addPivo(p);
                } catch (const std::exception& e) {
                    cerr << "Failed to read sheet '" << name << "': " << e.what() << endl;
                }
            }
        }

        doc.close();
    } catch (const std::exception& e) {
        cerr << "Error opening document: " << e.what() << endl;
    }
}