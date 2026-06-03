#include "tree.hpp"
#include "json.hpp"
#include <iostream>
#include <fstream>

using json = nlohmann::json;
using namespace std;

void Tree::destruir(TreeNode *node)
{ // libera de forma recursiva la memoria de un nodo y todos sus hijos, siguiendo firstChild y nextSibling, que se llama desde el destructor
    if (node == nullptr)
        return;
    destruir(node->firstChild);
    destruir(node->nextSibling);
    delete node;
}

void Tree::insertar(const Book &book)
{ // se crea un nuevo nodo con los datos del libro recibido y se inserta al final de la lista de hijos de la raíz
    TreeNode *nuevo = new TreeNode();
    nuevo->data = book;
    nuevo->firstChild = nullptr;
    nuevo->nextSibling = nullptr;
    if (root->firstChild == nullptr)
    { // si la raíz no tiene hijos, el nuevo nodo es el primero
        root->firstChild = nuevo;
        return;
    }
    TreeNode *actual = root->firstChild; // si hay hijos, se recorre hasta el último y se enlaza
    while (actual->nextSibling != nullptr)
    {
        actual = actual->nextSibling;
    }
    actual->nextSibling = nuevo;
}

void Tree::listar_recursivo(TreeNode *node)
{ // se recorre el arbol en preorder imprimiendo el ID de cada nodo
    if (node == nullptr)
        return;
    cout << node->data.id << "\n";
    listar_recursivo(node->firstChild);
    listar_recursivo(node->nextSibling);
}

bool Tree::es_precursor(const Book &book)
{ // se verifica si un libro es precursor y retorna falso si el libro no tiene similares o si su año de publicacion es desconocido
    if (book.ano_pub == -1)
        return false;
    if (book.similares.empty())
        return false;
    for (const SimilarBook &similar : book.similares)
    {
        if (similar.ano_pub == -1)
            return false;
        if (similar.ano_pub <= book.ano_pub)
            return false;
    }
    return true;
}

void Tree::precursores_recursivo(TreeNode *node)
{ // recorre el arbol e imprime el id de los libros precursores
    if (node == nullptr)
        return;
    if (es_precursor(node->data))
    {
        cout << node->data.id << "\n";
    }
    precursores_recursivo(node->firstChild);
    precursores_recursivo(node->nextSibling);
}

TreeNode *Tree::borrar_ratings_recursivo(TreeNode *node, float r)
{ // elimina recursivamente los nodos con rating menor al limite r, y retorna el nodo resultante después de posibles eliminaciones para que el nodo padre peuda reconectar sus punteros
    if (node == nullptr)
        return nullptr;
    node->nextSibling = borrar_ratings_recursivo(node->nextSibling, r); // primero se procesan los hermanos y luego se decide sobre el nodo actual
    node->firstChild = borrar_ratings_recursivo(node->firstChild, r);
    if (node->data.rating <= r)
    { // si el rating del nodo actual es menor al limite r, se elimina
        TreeNode *siguiente = node->nextSibling;
        delete node;
        return siguiente;
    }

    return node;
}

Tree::Tree(const string &json_path)
{                          // se define el constructor del arbol, donde se lee el json generado por procesarlibros.py y se construye el arbol insertando cada libro como hijo de la raíz
    root = new TreeNode(); // se crea el nodo raíz vacío
    root->firstChild = nullptr;
    root->nextSibling = nullptr;
    ifstream archivo(json_path); // se abre el json
    if (!archivo.is_open())
    {
        cerr << "Error: no se pudo abrir el archivo " << json_path << "\n";
        return;
    }
    json data; // se procesa el json
    archivo >> data;
    for (const auto &j : data)
    { // se recorre cada libro del json y se construye el arbol
        Book book;
        book.id = j["id"].get<string>();
        book.titulo = j["titulo"].get<string>();
        book.isbn = j["isbn"].get<string>();
        book.idioma = j["idioma"].get<string>();
        book.descripcion = j["descripcion"].get<string>();
        book.ano_pub = j["ano_pub"].is_null() ? -1 : j["ano_pub"].get<int>();
        book.num_paginas = j["num_paginas"].is_null() ? -1 : j["num_paginas"].get<int>();
        book.rating = j["rating"].is_null() ? 0.0 : j["rating"].get<float>();
        for (const auto &s : j["similares"])
        {
            SimilarBook similar; // se cargan los libros similares
            similar.titulo = s["titulo"].get<string>();
            similar.isbn = s["isbn"].get<string>();
            similar.ano_pub = s["ano_pub"].is_null() ? -1 : s["ano_pub"].get<int>();
            book.similares.push_back(similar);
        }

        insertar(book);
    }

    cout << "Arbol construido con " << data.size() << " libros.\n";
}

Tree::~Tree()
{ // se define el destructor del arbol, que libera la memoria usada por los nodos
    destruir(root->firstChild);
    delete root;
}

void Tree::listar()
{ // se recorre el arbol en preorder e imprime los id de los libros
    cout << "IDs en preorder:\n";
    listar_recursivo(root->firstChild);
}

void Tree::borrar_ratings(float r)
{ // se elimina del arbol todos los libros con rating menor o igual a r
    root->firstChild = borrar_ratings_recursivo(root->firstChild, r);
    cout << "Libros con rating <= " << r << " eliminados.\n";
}

void Tree::precursores()
{ // imprime los id de los libros que solo tienen libros similares publicados en años posteriores al libro padre
    cout << "IDs de libros precursores:\n";
    precursores_recursivo(root->firstChild);
}
