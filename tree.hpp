#ifndef TREE_H
#define TREE_H
 
#include <string>
#include <vector>
#include <fstream>

struct SimilarBook {    //Similarbook, representa a un libro que sea similar al principal, guardando su titulo, isbn y año de publicacion
    std::string titulo;
    std::string isbn;
    int ano_pub;
};

struct Book {      //Book representa a un libro dentro del dataset con sus datos
    std::string id;
    std::string titulo;
    std::string isbn;
    std::string idioma;
    std::string descripcion;
    int ano_pub;
    int num_paginas;
    float rating;
    std::vector<SimilarBook> similares;
};

struct TreeNode {   //Treenode es el nodo del arbol general
    Book data;
    TreeNode* firstChild;   
    TreeNode* nextSibling;  
};

class Tree {    //esta clase, construye el arbol al leer un archivo books.json, generado por procesarlibros.py
private:
    TreeNode* root; //puntero al nodo raíz del árbol
    void destruir(TreeNode* node);  //se libera la memoria de un nodo y de sus descendientes de forma recursiva
    void insertar(const Book& book);    //se inserta un libro como hijo de la raiz
    void listar_recursivo(TreeNode* node);  //se recorre el árbol en preorder imprimiento los id
    TreeNode* borrar_ratings_recursivo(TreeNode* node, float r);    //se elimina de forma recursiva los nodos con rating menor o igual al límite, y retorna el nodo resultante despues de posibles eliminaciones
    void precursores_recursivo(TreeNode* node); //se recorre el arbol buscando libros precursores
    bool es_precursor(const Book& book);    //se verifica si todos los libros similares de algún libro, fueron publicados en años posteriores al libro padre

public:
    Tree(const std::string& json_path);    //constructor para leer el json y construir el arbol
    ~Tree();    //destructor para liberar la memoria del arbol
    void listar();     //se recorre el arbol en preorder y se imprimen los id de los libros 
    void borrar_ratings(float r);   //se elimina del arbol, todos los libros con rating menor o igual al limite r
    void precursores(); //se imprimen los id de los libros que solo tienen libros similares publicados en años posteriores
};

#endif