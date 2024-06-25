#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

// Funciones auxiliares para obtener datos del usuario
string obtenerCadena(const string& mensaje) {
    string valor;
    cout << mensaje;
    getline(cin, valor);
    return valor;
}

double obtenerDouble(const string& mensaje) {
    double valor;
    string input;
    while (true) {
        cout << mensaje;
        getline(cin, input);
        try {
            valor = stod(input);
            if (valor >= 0) {
                break;
            } else {
                cout << "Entrada invalida. Por favor, ingrese un numero valido." << endl;
            }
        } catch (const exception& e) {
            cout << "Entrada invalida. Por favor, ingrese un numero valido." << endl;
        }
    }
    return valor;
}

int obtenerEntero(const string& mensaje) {
    int valor;
    string input;
    while (true) {
        cout << mensaje;
        getline(cin, input);
        try {
            valor = stoi(input);
            if (valor >= 0) {
                break;
            } else {
                cout << "Entrada invalida. Por favor, ingrese un numero entero positivo." << endl;
            }
        } catch (const exception& e) {
            cout << "Entrada invalida. Por favor, ingrese un numero entero valido." << endl;
        }
    }
    return valor;
}

// Clase base Producto
class Producto {
protected:
    string nombre;
    double precio;
    string codigo;
public:
    Producto(const string& n, double p, const string& c) : nombre(n), precio(p), codigo(c) {}

    virtual void mostrarDetalles() const = 0;

    string obtenerCodigo() const {
        return codigo;
    }

    string obtenerNombre() const {
        return nombre;
    }

    double obtenerPrecio() const {
        return precio;
    }

    void editarDetalles(const string& n, double p) {
        nombre = n;
        precio = p;
    }

    virtual string obtenerTipo() const = 0;

    virtual string obtenerAtributo() const = 0;

    virtual void editarDetalles(const string& n, double p, const string& atributo) = 0;

    virtual ~Producto() {}
};

// Clase derivada para Libros
class Libro : public Producto {
    string autor;
public:
    Libro(const string& n, double p, const string& c, const string& a) : Producto(n, p, c), autor(a) {}

    string obtenerAutor() const {
        return autor;
    }

    void mostrarDetalles() const override {
        cout << "\tLibro" << "\nNombre: "  << nombre << "\nPrecio: " << precio << "\nCodigo: " << codigo << "\nAutor: " << autor << endl;
    }

    string obtenerTipo() const override {
        return "Libro";
    }

    string obtenerAtributo() const override {
        return autor;
    }

    void editarDetalles(const string& n, double p, const string& a) override {
        Producto::editarDetalles(n, p);
        autor = a;
    }
};

// Clase derivada para Electronicos
class Electronico : public Producto {
    string marca;
public:
    Electronico(const string& n, double p, const string& c, const string& m) : Producto(n, p, c), marca(m) {}

    string obtenerMarca() const {
        return marca;
    }

    void mostrarDetalles() const override {
        cout << "Electronico - Nombre: " << nombre << ", Precio: " << precio << ", Codigo: " << codigo << ", Marca: " << marca << endl;
    }

    string obtenerTipo() const override {
        return "Electronico";
    }

    string obtenerAtributo() const override {
        return marca;
    }

    void editarDetalles(const string& n, double p, const string& m) override {
        Producto::editarDetalles(n, p);
        marca = m;
    }
};

// Clase derivada para Ropa
class Ropa : public Producto {
    string talla;
public:
    Ropa(const string& n, double p, const string& c, const string& t) : Producto(n, p, c), talla(t) {}

    string obtenerTalla() const {
        return talla;
    }

    void mostrarDetalles() const override {
        cout << "Ropa - Nombre: " << nombre << ", Precio: " << precio << ", Codigo: " << codigo << ", Talla: " << talla << endl;
    }

    string obtenerTipo() const override {
        return "Ropa";
    }

    string obtenerAtributo() const override {
        return talla;
    }

    void editarDetalles(const string& n, double p, const string& t) override {
        Producto::editarDetalles(n, p);
        talla = t;
    }
};

template <typename T>
class Inventario {
private:
    vector<T*> productos;
public:
    bool agregar(T* producto) {
        for (T* prod : productos) {
            if (prod->obtenerCodigo() == producto->obtenerCodigo()) {
                cout << "Producto con el mismo codigo ya existe: " << producto->obtenerCodigo() << endl;
                return false;
            }
        }
        productos.push_back(producto);
        return true;
    }

    bool guardarEnArchivo(const string& nombreArchivo) const {
        ofstream archivo(nombreArchivo);
        if (!archivo) {
            cout << "No se pudo abrir el archivo para escribir." << endl;
            return false;
        }

        for (const T* prod : productos) {
            archivo << prod->obtenerTipo() << ",";
            archivo << prod->obtenerNombre() << ",";
            archivo << prod->obtenerPrecio() << ",";
            archivo << prod->obtenerCodigo() << ",";
            archivo << prod->obtenerAtributo();
            archivo << endl;
        }

        archivo.close();
        return true;
    }

    void eliminar(const string& codigo) {
        for (auto it = productos.begin(); it != productos.end(); ++it) {
            if ((*it)->obtenerCodigo() == codigo) {
                delete *it;
                productos.erase(it);
                cout << "Producto eliminado." << endl;
                return;
            }
        }
        cout << "Producto no encontrado." << endl;
    }

    T* buscar(const string& codigo) const {
        for (T* prod : productos) {
            if (prod->obtenerCodigo() == codigo) {
                return prod;
            }
        }
        return nullptr;
    }

    void listar() const {
        for (T* prod : productos) {
            prod->mostrarDetalles();
        }
    }

    bool cargarDesdeArchivo(const string& nombreArchivo) {
        ifstream archivo(nombreArchivo);
        if (!archivo) {
            cout << "No se pudo abrir el archivo para leer." << endl;
            return false;
        }

        // Limpiar el inventario actual antes de cargar el nuevo
        for (T* prod : productos) {
            delete prod;
        }
        productos.clear();

        string linea;
        while (getline(archivo, linea)) {
            string tipo, nombre, codigo, atributo;
            size_t pos = 0;

            pos = linea.find(",");
            tipo = linea.substr(0, pos);
            linea.erase(0, pos + 1);

            pos = linea.find(",");
            nombre = linea.substr(0, pos);
            linea.erase(0, pos + 1);

            pos = linea.find(",");
            double precio = stod(linea.substr(0, pos));
            linea.erase(0, pos + 1);

            pos = linea.find(",");
            codigo = linea.substr(0, pos);
            linea.erase(0, pos + 1);

            atributo = linea;

            T* producto = nullptr;
            if (tipo == "Libro") {
                producto = new Libro(nombre, precio, codigo, atributo);
            } else if (tipo == "Electronico") {
                producto = new Electronico(nombre, precio, codigo, atributo);
            } else if (tipo == "Ropa") {
                producto = new Ropa(nombre, precio, codigo, atributo);
            } else {
                cout << "Tipo de producto desconocido: " << tipo << endl;
                continue;
            }
            productos.push_back(producto);
        }
        archivo.close();
        return true;
    }

    void editar(const string& codigo) {
        T* producto = buscar(codigo);
        if (!producto) {
            cout << "Producto no encontrado." << endl;
            return;
        }
        string nombre = obtenerCadena("Nuevo nombre: ");
        double precio = obtenerDouble("Nuevo precio: ");

        if (producto->obtenerTipo() == "Libro") {
            string autor = obtenerCadena("Nuevo autor: ");
            dynamic_cast<Libro*>(producto)->editarDetalles(nombre, precio, autor);
        } else if (producto->obtenerTipo() == "Electronico") {
            string marca = obtenerCadena("Nueva marca: ");
            dynamic_cast<Electronico*>(producto)->editarDetalles(nombre, precio, marca);
        } else if (producto->obtenerTipo() == "Ropa") {
            string talla = obtenerCadena("Nueva talla: ");
            dynamic_cast<Ropa*>(producto)->editarDetalles(nombre, precio, talla);
        }

        cout << "Producto editado exitosamente." << endl;
    }

    ~Inventario() {
        for (T* prod : productos) {
            delete prod;
        }
    }
};

// Funcion para mostrar el menu
void mostrarMenu() {
    cout << "1. Crear Libro" << endl;
    cout << "2. Crear Electronico" << endl;
    cout << "3. Crear Ropa" << endl;
    cout << "4. Mostrar productos creados" << endl;
    cout << "5. Eliminar producto" << endl;
    cout << "6. Buscar producto" << endl;
    cout << "7. Guardar inventario en archivo" << endl;
    cout << "8. Cargar inventario desde archivo" << endl;
    cout << "9. Editar producto" << endl;
    cout << "10. Salir" << endl;
    cout << "Seleccione una opcion: ";
}

int main() {
    Inventario<Producto> inventario;

    int opcion;
    do {
        system("cls");
        mostrarMenu();
        opcion = obtenerEntero("");

        if (opcion >= 1 && opcion <= 3) {
            string nombre = obtenerCadena("Nombre: ");
            double precio = obtenerDouble("Precio: ");
            string codigo;

            switch (opcion) {
                case 1: {
                    do {
                        codigo = obtenerCadena("Codigo (empiece con 'L'): ");
                        if (codigo.size() == 0 || codigo[0] != 'L') {
                            cout << "Codigo invalido. Debe empezar con 'L'." << endl;
                        }
                    } while (codigo.size() == 0 || codigo[0] != 'L');

                    string autor = obtenerCadena("Autor: ");
                    Libro* nuevoLibro = new Libro(nombre, precio, codigo, autor);
                    if (!inventario.agregar(nuevoLibro)) {
                        delete nuevoLibro;
                    }
                    break;
                }
                case 2: {
                    do {
                        codigo = obtenerCadena("Codigo (empiece con 'E'): ");
                        if (codigo.size() == 0 || codigo[0] != 'E') {
                            cout << "Codigo invalido. Debe empezar con 'E'." << endl;
                        }
                    } while (codigo.size() == 0 || codigo[0] != 'E');

                    string marca = obtenerCadena("Marca: ");
                    Electronico* nuevoElectronico = new Electronico(nombre, precio, codigo, marca);
                    if (!inventario.agregar(nuevoElectronico)) {
                        delete nuevoElectronico;
                    }
                    break;
                }
                case 3: {
                    do {
                        codigo = obtenerCadena("Codigo (empiece con 'R'): ");
                        if (codigo.size() == 0 || codigo[0] != 'R') {
                            cout << "Codigo invalido. Debe empezar con 'R'." << endl;
                        }
                    } while (codigo.size() == 0 || codigo[0] != 'R');

                    string talla;
                    do {
                        talla = obtenerCadena("Talla (s, m, l): ");
                        if (talla != "s" && talla != "m" && talla != "l") {
                            cout << "Talla invalida. Ingrese una talla valida (s, m, l)." << endl;
                        }
                    } while (talla != "s" && talla != "m" && talla != "l");

                    Ropa* nuevaRopa = new Ropa(nombre, precio, codigo, talla);
                    if (!inventario.agregar(nuevaRopa)) {
                        delete nuevaRopa;
                    }
                    break;
                }
            }
        } else if (opcion == 4) {
            system("cls");
            inventario.listar();
            cout << "Presione cualquier tecla para continuar...";
            cin.ignore();
            cin.get();
        } else if (opcion == 5) {
            system("cls");
            string codigo = obtenerCadena("Ingrese el codigo del producto a eliminar: ");
            inventario.eliminar(codigo);
            cout << "Presione cualquier tecla para continuar...";
            cin.ignore();
            cin.get();
        } else if (opcion == 6) {
            system("cls");
            string codigo = obtenerCadena("Ingrese el codigo del producto a buscar: ");
            Producto* producto = inventario.buscar(codigo);
            if (producto) {
                producto->mostrarDetalles();
            } else {
                cout << "Producto no encontrado." << endl;
            }
            cout << "Presione cualquier tecla para continuar...";
            cin.ignore();
            cin.get();
        } else if (opcion == 7) {
            system("cls");
            string nombreArchivo = obtenerCadena("Ingrese el nombre del archivo: ");
            if (inventario.guardarEnArchivo(nombreArchivo)) {
                cout << "Inventario guardado exitosamente." << endl;
            } else {
                cout << "Error al guardar el inventario." << endl;
            }
            cout << "Presione cualquier tecla para continuar...";
            cin.ignore();
            cin.get();
        } else if (opcion == 8) {
            system("cls");
            string nombreArchivo = obtenerCadena("Ingrese el nombre del archivo: ");
            if (inventario.cargarDesdeArchivo(nombreArchivo)) {
                cout << "Inventario cargado exitosamente." << endl;
                inventario.listar();  // Mostrar productos cargados
            } else {
                cout << "Error al cargar el inventario." << endl;
            }
            cout << "Presione cualquier tecla para continuar...";
            cin.ignore();
            cin.get();
        } else if (opcion == 9) {
            system("cls");
            string codigo = obtenerCadena("Ingrese el codigo del producto a editar: ");
            inventario.editar(codigo);
            cout << "Presione cualquier tecla para continuar...";
            cin.ignore();
            cin.get();
        } else if (opcion != 10) {
            system("cls");
            cout << "Opcion invalida." << endl;
            cout << "Presione cualquier tecla para continuar...";
            cin.ignore();
            cin.get();
        }
    } while (opcion != 10);

    system("cls");
    cout << "Gracias por usar el inventario. !Adios!" << endl;
    return 0;
}
