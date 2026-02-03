#include "../include/Instance.hpp"
#include <OpenXLSX.hpp>

using namespace OpenXLSX;
using namespace std;
using namespace opthirrygated;

Instance::Instance(string path) {
    setPath(path);
    Instance::exec();
}


template <typename T>
vector<T> loadColumn(XLWorksheet& wks, const string& col, int startCell, int endCell) {
    vector<T> values;
    for (int i = startCell; i <= endCell; i++) {
        string cellRef = col + to_string(i);
        values.push_back(wks.cell(XLCellReference(cellRef)).value().get<T>());
    }
    return values;
}

void Instance::exec() {

    try {
        XLDocument doc;
        doc.open(getPath());
        auto wks = doc.workbook().worksheet("ciclo");
        setCicle(loadColumn<float>(wks, "A", 2, 121));
        setPrec(loadColumn<float>(wks, "B", 2, 121));
        setEtc(loadColumn<float>(wks, "C", 2, 121));
        setCad(loadColumn<float>(wks, "D", 2, 121));
        setLc(loadColumn<float>(wks, "E", 2, 121));

        wks = doc.workbook().worksheet("perc");
        setPerc(loadColumn<int>(wks, "E", 2, 12));
        setCost(loadColumn<float>(wks, "D",2 , 12));
        setLamp(loadColumn<float>(wks, "C", 2, 12));


        double minLam = *min_element(lamp.begin(), lamp.end());
        double maxLam = *max_element(lamp.begin(), lamp.end());


        int lowThreshold = minLam+0.2*(maxLam-minLam);
        int maxLowLam = 0;
        for (int i=0; i<lamp.size(); i++) {
            if (lamp[i] <= lamp[lowThreshold]) {
                maxLowLam = lamp[i] > lamp[maxLowLam] ? i : maxLowLam ;
                lowLevels.push_back(i);
                continue;
            }
            highLevels.push_back(i);
        }


        setMaxLowLevel(maxLowLam);

    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

}

