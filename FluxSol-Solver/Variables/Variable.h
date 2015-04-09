#ifndef VARIABLE_H_INCLUDED
#define VARIABLE_H_INCLUDED


//Puedo probar esto
//O tambien las variables pueden ser derivadas
// Por ejemplo de vectorial y escalar, pero por ejemplo, la variable puede ser una clase
//eso lo hace mucho mas general
template <class T>
class _Variable{

    int Id;     //Id de la variable
    T valor;

    public:
    //Constructores
    _Variable(){};
    _Variable(const T){};

    T & Valor (){return valor;};


};

#endif // VARIABLE_H_INCLUDED
