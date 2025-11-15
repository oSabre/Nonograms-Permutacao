#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype> // Para std::isspace e std::isdigit

using namespace std;

/**
 * @brief Extrai e escreve APENAS números de uma linha, ignorando C++ syntax.
 */
void extract_numbers_to_file(const string& line, ofstream& output_file) {
    string cleaned_line = line;

    // 1. Remover caracteres que NÃO são dígitos, espaço em branco ou sinal de negativo
    // Isso deve remover '{', '}', ',', '//', e o nome da variável.
    cleaned_line.erase(
        remove_if(cleaned_line.begin(), cleaned_line.end(), 
            [](char c){ 
                return !isdigit(c) && !isspace(c) && c != '-'; 
            }), 
        cleaned_line.end()
    );

    // 2. Usar stringstream para extrair os inteiros da linha limpa
    stringstream ss(cleaned_line);
    int number;
    
    // Tenta extrair um inteiro de cada parte da linha
    while (ss >> number) {
        output_file << number << " ";
    }
}

/**
 * @brief Converte um arquivo C++ contendo um array de inteiros para um arquivo TXT simples.
 */
bool converter_cpp_para_txt(const string& input_path, const string& output_path) {
    ifstream input_file(input_path);
    ofstream output_file(output_path);
    
    if (!input_file.is_open() || !output_file.is_open()) {
        cerr << "Erro: Nao foi possivel abrir/criar os arquivos." << endl;
        return false;
    }

    string line;
    while (getline(input_file, line)) {
        // Ignora linhas de comentários ou declaração de variável (pode ser ajustado)
        if (line.find("const int") != string::npos || line.find("//") != string::npos) {
            continue; 
        }
        
        extract_numbers_to_file(line, output_file);
    }
    
    cout << "Sucesso! Convertido " << input_path << " para " << output_path << endl;
    return true;
}

int main(int argc, char* argv[]) {
    // ... (A lógica main() de controle de argumentos permanece a mesma) ...
    if (argc != 3) {
        cout << "Uso: ./converter <arquivo_de_entrada.cpp> <arquivo_de_saida.txt>" << endl;
        cout << "Exemplo: ./converter samples/webpbn-00001.cpp samples/webpbn-00001.txt" << endl;
        return 1;
    }

    string input_file = argv[1];
    string output_file = argv[2];

    return converter_cpp_para_txt(input_file, output_file) ? 0 : 1;
}