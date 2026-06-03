#include "tree.hpp"
#include <iostream>
 
int main() {
    Tree arbol("books.json");   //se construye el arbol desde el json
    std::cout << "\n--- listar() ---\n";    //imprime todos los id en preorder
    arbol.listar();
    std::cout << "\n--- precursores() ---\n";   //imprime los id de libros precursores
    arbol.precursores();
    std::cout << "\n--- borrar_ratings(3.0) ---\n";     //elimina los libros con rating menor o igual a 3.0
    arbol.borrar_ratings(3.0);
    std::cout << "\n--- listar() despues de borrar ---\n";  //para verificar que se borraron
    arbol.listar();
 
    return 0;
}
