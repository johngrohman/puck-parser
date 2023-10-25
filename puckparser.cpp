/*
    John Grohman
    Homework 5 - Recursive Descent Parser - Part 2
    Comp Sci 3500 - Dr. Morales
    puckparser.cpp
*/

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
using namespace std;

string prevtoken;
string token;
vector<array<string, 2>> symboltable;

bool getToken(string s);
bool is_digit(char s);
bool is_letter(char s);
bool is_Integer( string& s );
bool is_Decimal(string input);
bool is_String(string s);
bool is_Keyword(string s);
bool is_Identifier(string s);
void is_MulOperator();
void is_Relation();
void is_AddOperator();
void is_SimpleExpression();
void is_Expression();
void is_Factor();
void is_Term();
void is_Designator();
void is_Selector();
void is_Assignment();
void is_PrintStmt();
void is_Stmt(string s = "");
void is_StmtSeq(string s = "");
void is_FuncDec();
void is_DeclSeq();
void is_FuncBody();
void is_ParamSeq();
void is_RetrnStmt();
void is_IfStmt();
void is_FuncCall();
void printSymbolTable();
bool isInSymbolTable(string s, string t);

int main() {
    // Main Driver
    
    try {
        getToken("FIRST");
        is_DeclSeq();
        cout << "CORRECT" << endl;
        cout << "Symbol Table : " << "size " << symboltable.size() << endl;
        for (int i = 0; i < symboltable.size(); i++) {
            cout << symboltable[i][0] << " " << symboltable[i][1] << endl;
        }
    } catch (const char* e) {
        cout << "INVALID!" << endl;
        if (e == "Undefined function" ) {
            cout << e << " \"" << prevtoken << "\"" << endl;
        }  else if (e == "Variable used as a function") {
            cout << "Variable \"" << prevtoken << "\" used as function" << endl;
        } else if (e == "Function was used as a variable") {
            cout << "Function \"" << prevtoken << "\" used as a variable" << endl;
        } else if (e == "EOF") {
            cout << "\"END.\" expected, reached end of file." << endl;
        }
        else {
            cout << e << " expected, got \"" << token << "\"." << endl;
        }
    } 
    return 0;
}

bool isInSymbolTable(string s, string t) {
    for (int i = 0; i < symboltable.size(); i++) {
        if (symboltable[i][0] == s && symboltable[i][1] == t) {
            return true;
        }
    }
    return false;
}

void printSymbolTable() {
    for (int i = 0; i < symboltable.size(); i++) {
        cout << symboltable[i][0] << " " << symboltable[i][1] << endl;
    }
}

bool getToken(string s) {
    prevtoken = token;
    if (!(cin >> token)) {return false;}
    return true;
}

bool is_digit(char s){
    if (s == '0' || s == '1' || s == '2' || s == '3' || s == '4' || s == '5' || s == '6' || s == '7' || s == '8' || s == '9') {
        return true;
    }
    return false;
}

bool is_letter(char s) {
    if ((((int(s) >= 97 && int(s) <= 122) || (int(s) >= 65 && int(s) <= 90)))) {
        return true;
    }
    return false;
}

bool is_Integer( string& s ){
    int i = 1;
    int state = 0;
    if ((s[0] != '+') && (s[0] != '-') && (!is_digit(s[0]))) {
        return false;
    }
    while (i < s.length()) {
        if (!is_digit(s[i])) {
            return false;
        }
        i++;
    }
    return true;
}

bool is_Decimal(string s) {
    if ((s[0] != '+') && (s[0] != '-') && (!is_digit(s[0]))) {
        return false;
    }
    int found_decimal = 0;
    for(int i = 1; i < int(s.length())-1; i++) {
        if (!(isdigit(s[i])) && s[i] != '.') {
            return false;
        }
        if (s[i] == '.') {
            found_decimal++;
        }
        if (found_decimal > 1) {return false;}
    }
    return true;
}

bool is_String(string s) {
    // Validate the first and last character are quotes, while making sure the length of the input is greater than 2 (blankspace isn't allowed)
    if ((s[0] == '\"' && s[s.length()-1] == '\"') && (s.length() > 2)) {
        int i = 1;
        while (i < s.length() - 1) {
            if (s[i] == '\"') {
                return false;
            }
            i++;
        }
        return 1;
    }
    return 0;
}

// Tests if the input is a keyword
bool is_Keyword(string s) {
    if (s == "PRINT" || s == "." || s == "]" || s == "[" || s == "," || s == "(" || s == ")" || s == ";" || s == "WHILE" || s == "ENDW" || s == "RETURN" || s == "IF" || s == "ELSE" || s == "ENDIF" || s == "END.") {
        return true;
    } else {
        return false;
    }
}

// Tests if the input is an operator
bool is_Operator(string s) {
    if (s == ":=" || s == "~" || s == "<" || s == ">" || s == "=" || s == "#" || s == "+" || s == "-" || s == "&" || s == "OR" || s == "*" || s == "/" || s == "AND") {
        return true;
    } else {
        return false;
    }
}
 
// Tests if the input is an identifier
bool is_Identifier(string s) {
    if (is_Keyword(s) || !is_letter(s[0])) {
        return false;
    }
    int i = 1;
    while(i < s.length() -1) {
        if (!is_letter(s[i]) && !is_digit(s[i])) {
            return false;
        }
        i++;
    }
    return true;
}

// Tests if the input is a relation
void is_Relation() {
    if (token == "<" || token == ">" || token == "=" || token == "#") {
        getToken("REL");
    }
    else throw("Expected Relation");
}

// Tests if the input is an add operator
void is_AddOperator() {
    if (token == "+" || token == "-" || token == "=" || token == "#" || token == "&" || token =="OR") {
        getToken("ADD");
    }
    else throw("Expected AddOperator");
}

// Tests if the input is a mul operator
void is_MulOperator() {
    if ( token == "*" || token == "/" || token == "AND" || token == "DIV" || token == "MOD") {
        getToken("MUL");
    } else throw("Expected MulOperator");
}

void is_SimpleExpression() {
    is_Term();
    while(token == "+" || token == "-" || token == "OR" || token == "&") {
        is_AddOperator();
        // getToken("ADD");
        is_Term();
        // getToken("TERM");
    }
}

void is_Expression() {
    is_SimpleExpression();

    while(token == "<" || token == ">" || token == "=" || token == "#") {
        is_Relation();
        // getToken("REL");
        is_SimpleExpression();
        // getToken("TERM");
    }
}

void is_Factor() {
    if ( is_Integer(token)) {
        getToken("INT");
    } else if (is_Decimal(token)) {
        getToken("DEC");
    } else if (is_String(token)) {
        getToken("STR");
    } else if ( token == "(" ) {
        getToken("LPAREN");
        is_Expression();
        if ( token == ")" ) {
            getToken("RPAREN");
        } else throw("\")\"");
    }
    else if ( token == "~" ){
        getToken("NOT");
        is_Factor();
    } else if (is_Identifier(token)) {
        if (isInSymbolTable(token, "function")) {
            is_FuncCall();
        } else {
            is_Designator();
        }

    }
    else throw("Factor");
}

void is_Term() {
    is_Factor();
    while (token == "*" || token == "/" || token == "AND" || token == "DIV" || token == "MOD") {
        is_MulOperator();
        is_Factor();
    }
}

void is_Designator() {
    if (is_Identifier(token)) {
        getToken("ID in designator");
        if (token == "(") {
            if (isInSymbolTable(prevtoken, "variable")) {
                throw("Variable used as a function");
            } else if (isInSymbolTable(prevtoken, "function")) {
                throw("Function used as a variable");
            } else {
                throw("Undefined function");
            }
        }

        array<string, 2> func = {prevtoken, "variable"};
        if (find(symboltable.begin(), symboltable.end(), func) == symboltable.end()) {
            symboltable.push_back(func);        
        }


        while (token == "." || token == "[") {
            is_Selector();
        }
    } else {
        throw("Identifier");
    }
}

void is_Selector() {
    if (token == ".") {
        getToken("DOT");
        if (is_Identifier(token)) {            
            if (!isInSymbolTable(token, "variable")) {
                array<string, 2> func = {token, "variable"};
                symboltable.push_back(func);        
            }
            getToken("ID");
        }
    } else if (token == "[") {
        getToken("LBRACK");
        is_Expression();
        if (token == "]") {
            getToken("RBRACK");
            is_Selector();
        } else {
            throw("\"]\"");
        }
    }
}

void is_ParamSeq() {
    if (!is_Identifier(token)) {
        throw("Identifier");
    }
    array<string, 2> func = {token, "variable"};
    if (find(symboltable.begin(), symboltable.end(), func) == symboltable.end()) {
        symboltable.push_back(func);        
    }
    getToken("ID");
    while (token == ",") {

        getToken("COMMA");
        if (is_Identifier(token)) {
            getToken("ID");
        } else {
            throw("Identifier");
        }
    }
}

void is_FuncCall() {
    if (is_Identifier(token)) {
        getToken("ID");
        if (token != "(") {
            throw("Function was used as a variable");
        }
        if (token == "(") {
            getToken("LPAREN");
            if (token != ")") {
                is_ParamSeq();
                getToken("ID");                
            } else if (token == ")") {
                getToken("RPAREN");
            } else {
                throw("\")\"");
            }
        } else {
            throw("\"(\"");
        }
    } else {
        throw("Identifier");
    }
}

void is_Assignment() {
    is_Designator();
    if (token == ":=") {
        getToken("ASSIGN");
        is_Expression();
    } else {
        throw("\":=\"");
    } 
}

void is_PrintStmt() {
    if ( token == "PRINT" ) {
        getToken("PRINT");
        if( token == "(" ) {
        getToken("LPAREN");
        is_Expression();
        if( token == ")" ){
            getToken("RPAREN");
        }
        else throw("\")\"");
        }
        else throw("\"(\"");
    }
    else throw("PRINT");
}

void is_IfStmt() {
    if (token == "IF") {
        getToken("IF");
        is_Expression();
        if (token == "THEN") {
            getToken("THEN");
            is_StmtSeq();
            if (token == "ELSE") {
                getToken("ELSE");
                is_StmtSeq();
            }
            if (token == "ENDIF") {
                getToken("ENDIF");
            } else {
                throw("ENDIF");
            }
        } else {
            throw("THEN");
        }
    } else {
        throw("IF");
    }
}

void is_WhileStmt() {
    if (token == "WHILE") {
        getToken("WHILE");
        is_Expression();
        if (token == "DO") {
            getToken("DO");
            is_StmtSeq();
            if (token == "ENDW") {
                getToken("ENDW");
            } else {
                throw("ENDW");
            }
        } else {
            throw("DO");
        }
    } else {
        throw("WHILE");
    }
}

void is_RetrnStmt() {
    if (token == "RETURN") {
        getToken("RETURN");
        if (token == "(") {
            getToken("LPAREN");
            if (is_Identifier(token)) {
                getToken("ID");
            } else {
                throw("Identifier");
            }
            if (token == ")") {
                getToken("RPAREN");
            } else {
                throw("\")\"");
            }
        } else {
            throw("\"(\"");
        }
    } else {
        throw("RETURN");
    }
}

// ******************************************************

void is_Stmt(string s) {
    array<string, 2> func = {token};
    if ( is_Identifier( token ) && !isInSymbolTable(token, "function")) is_Assignment();
    else if ( token == "PRINT" ) is_PrintStmt();
    else if ( token == "WHILE" ) is_WhileStmt();
    else if ( token == "RETURN" ) is_RetrnStmt();
    else if ( token == "IF" ) is_IfStmt();
    else if (is_Identifier( token ) && isInSymbolTable(token, "function")) is_FuncCall();
    else
        throw("Statement");
}

// ******************************************************

void is_StmtSeq(string s) {
    is_Stmt();
    while ( token == ";" ) {
        getToken("SEMICOLON in StmtSeq");
        is_Stmt();
    }
}

void is_FuncDec() {
    if ( token == "FUNCTION") {
        getToken("FUNCTION");
            array<string, 2> func = {token, "function"};
            symboltable.push_back(func);
        if (is_Identifier(token)) {
            getToken("ID in FuncDec");
            if (token == "(") {
                getToken("LPAREN");
                if (token != ")") {
                    is_ParamSeq();
                    getToken("ID");
                } else if (token == ")") {
                    getToken("RPAREN");
                } else {
                    throw("\")\"");
                }
                is_FuncBody();
            } else {
                throw("\"(\"");
            }
        } else {
            throw("Identifier");
        }
    } else {
        throw("Function Declaration");
    }
}

void is_FuncBody() {
    if (token == ";") {
        getToken("SEMICOLEN IN FUNC BODY");
    } else {
        is_StmtSeq("FUNCTION");
        if (token == "END.") {
            getToken("END.");
        } else {
            if (!getToken("again")){throw("EOF");}
            throw("\"END.\"");
        }
    }
}

void is_DeclSeq() {
    if (token != "FUNCTION") {
        throw("Function declaration");
    }
    while ( token == "FUNCTION" ) {
        is_FuncDec();
    }
}