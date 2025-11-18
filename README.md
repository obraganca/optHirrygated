
---

# **optHirrygated**

Projeto em **C++17** para execução de heurísticas construtivas e métodos de otimização. O sistema utiliza **IBM ILOG CPLEX**, **OpenXLSX** e **CMake** para compilação.

---

## 📌 Requisitos

* **C++17** (GCC 9+ ou Clang 10+)
* **CMake 3.10+**
* **IBM ILOG CPLEX 22.1.1**
* Biblioteca **OpenXLSX** (incluída no diretório `lib/OpenXLSX`)

O `CMakeLists.txt` assume CPLEX instalado em:

```
/opt/ibm/ILOG/CPLEX_Studio2211/
```

---

## 📦 Como compilar

No diretório raiz do projeto:

```bash
mkdir build
cd build
cmake ..
make
```

O executável será gerado como:

```
./optHirrygated
```

---

## ▶️ Como executar

Dentro da pasta `build`:

```bash
./optHirrygated
```

---

## 📁 Estrutura resumida

```
src/       # Código-fonte .cpp
include/   # Headers .hpp
lib/       # OpenXLSX
CMakeLists.txt
```

---
