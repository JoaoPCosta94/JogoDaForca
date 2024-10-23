// JogoDaForca.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <conio.h>
#include <Windows.h>
#include <string>
#include <vector>

using namespace std;

#define ESC 27
#define UP 72
#define DOWN 80
#define LEFT 75
#define ENTER 13

//======================================== Protótipos ========================================
int menu();
void umJogador();
void randPalavra(string palavra, vector <string>& PALAVRAS, int& indice, string& adivinha);
void escondePalavra(string adivinha, vector <char>& ESCONDIDAS);
void verificarVitoria(vector <char> ESCONDIDAS, bool& ganhou);
void desenharBoneco(int erros);
void gotoxy(int x, int y);
void sair();

int main()
{
	SetConsoleOutputCP(1252);
	gotoxy(50, 10);
	while (true)
	{
		int escolha = menu();

		switch (escolha)
		{
		case 0: umJogador();  break;
		case 3: sair(); break;
		}

	}
	

	

	
}
//==============================================================================================================
//Menu
int menu() {

	int setas = 0;
	int escolha = 0;
	string OP[4] = { "Um Jogador", "Dois Jogadores", "Opções", "Sair" };
	system("cls");
	while (true) {  // Loop para a navegação no menu
		
		for (int i = 0; i < 4; i++) {
			gotoxy(40, 5 + i);
			if (i == escolha) {
				gotoxy(50, 5 + i);  // Para posicionar corretamente a seta
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
				cout << ">> " << OP[i] << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			else {
				gotoxy(50, 5 + i);
				cout << "   " <<  OP[i];   // precisa de um espaço para alinhar porque a escolha da opçao empurra o texto
			}
		}
		gotoxy(35, 10);
		cout << "   Utilize as setas direcionais para escolher" << endl;

		setas = _getch();
		if (setas == 224) {
			setas = _getch();  
		}

		// Controle de navegação no menu
		switch (setas) {
		case UP:
			if (escolha - 1 >= 0) {   // Verifica o limite 
				escolha -= 1;  
			}
			break;
		case DOWN:
			if (escolha + 1 < 4) {  // Verifica o limite 
				escolha += 1;  
			}
			break;
		case ENTER:
			return escolha;  // Retorna a escolha final quando o ENTER for pressionado
		}
	}
}
//==============================================================================================================
// Função do jogo da forca para um só jogador
void umJogador() 
{
	int indice = 0;
	string adivinha;				 		 // Vai armazenar a palavra a adivinhar
	vector <string> PALAVRAS;				 // 'Pool' de onde vamos tirar uma palavra aleatoria
	vector <string> NOMES;					 // Nomes dos jogadores para podermos controlar os pontos
	vector <string> letrasErradas;           // Armazena as tentativas erradas
	vector <char> ESCONDIDAS;				 // Vai armazenar os "_" e substituir pela letra certa se o jogador adivinhar
	vector <char> TENTATIVAS;				 // Para mostrar as letras erradas que o jogador usou
	string palavra;
	string player;
	bool acertou = false;
	bool novoNome = false;		
	bool ganhou = false;			
	char letra;
	int erros = 0;
	
	randPalavra(palavra, PALAVRAS, indice, adivinha);					

	system("cls");
	gotoxy(50, 5);

	ifstream fread;
	fread.open("nomes.txt");
	while (getline (fread, player))						// Preencher o vector com os nomes
	{
		NOMES.push_back(player);
	}
	cout << "Nome: ";									// Pedir o nome ao jogador
	getline(cin, player);
	for (int i = 0; i < NOMES.size(); i++)				// Verificar se é um jogador novo e acrescentar à lista se assim for
	{
		if (player != NOMES[i])
		{
			novoNome = true;
		}
	}
	if (novoNome = true)
	{
		NOMES.push_back(player);
		ofstream fwrite;
		fwrite.open("nomes.txt", ios::app);			//ios::app para acrescentar ao ficheiro, porque senao substitui o que la está pelo que vou escrever
		fwrite << "\n" << player;
		fwrite.close();
	}
	fread.close();
	
	system("cls");
	gotoxy(50, 5);

	cout << "Bem-vindo(a) " << player << "!";

	Sleep(3000);
	system("cls");

	escondePalavra(adivinha, ESCONDIDAS);
	
	do
	{
		system("cls");
		acertou = false;									
		desenharBoneco(erros);
		cout << endl;
		for (char letra : ESCONDIDAS) {
			cout << letra << " ";
		}
		
		cout << endl;
		cout << "ERRADAS: ";
		for (char letra : TENTATIVAS) {
			cout << letra << " ";
		}
		cout << endl;
		if (ganhou == true)
		{
			system("cls");
			gotoxy(50, 5);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
			cout << "Parabéns " << player << " ganhou!";
			gotoxy(50, 6);
			cout << "A palavra era ";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
			cout << adivinha;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			Sleep(3000);
			break;
		}
		cout << "Escolha uma letra:  ";
		letra = toupper(getchar());
		cin.clear();
		cin.ignore(100, '\n');
		for (int i = 0; i < adivinha.size(); i++)
		{
			if (letra == adivinha[i])
			{
				ESCONDIDAS[i] = letra;
				acertou = true;
			}
		}
		if (acertou == false)
		{
			TENTATIVAS.push_back(letra);
			erros++;
		}
		verificarVitoria(ESCONDIDAS, ganhou);
		
		
		

	} while (erros <= 7 || ganhou == false);
	
}
//==============================================================================================================
// Função para escolher uma palavra aleatória
void randPalavra(string palavra, vector <string> &PALAVRAS, int &indice, string &adivinha) {


	ifstream fread;
	fread.open("palavras.txt");

	while (getline(fread, palavra))
	{
	PALAVRAS.push_back(palavra);				      	// Push back a cada linha do palavras.txt para dentro do vector
	}

	srand(time(0));
	indice = (rand() % PALAVRAS.size());				// Para dar um numero aleatório até ao limite do vector que vai ser usado como indice do mesmo

	adivinha = PALAVRAS[indice];						// Para guardar a palavra aleatória numa variavel separada
	fread.close();
}
//==============================================================================================================
// Função para esconder a palavra 
void escondePalavra(string adivinha, vector <char> &ESCONDIDAS)
{
	for (int i = 0; i < adivinha.size(); i++)
	{
		ESCONDIDAS.push_back('_');					// Guardar um vector com '_' que vai dizer a quantidade de letras escondidas que a palavra a adivinhar tem
	}

}
//==============================================================================================================
// Função para verificar se ganhou
void verificarVitoria(vector <char> ESCONDIDAS, bool &ganhou)
{
	ganhou = true;
	for (int i = 0; i < ESCONDIDAS.size(); i++)
	{
		if (ESCONDIDAS[i] == '_')
		{
			ganhou = false;
		}
	}
}
	
//==============================================================================================================
// Função para desenhar o boneco 
void desenharBoneco(int erros)
{
	if (erros == 0)
	{
		cout << "  +---+ \n";
		cout << "  |   | \n";
		cout << "  |    \n";
		cout << "  |    \n";
		cout << "  |    \n";
		cout << "  |    \n";
		cout << "========= \n";
	}
	else if (erros == 1)
	{
		cout << "  +---+ \n";
		cout << "  |   | \n";
		cout << "  |   O \n";
		cout << "  |    \n";
		cout << "  |    \n";
		cout << "  |    \n";
		cout << "========= \n";
	}
	else if (erros == 2)
	{
		cout << "  +---+ \n";
		cout << "  |   | \n";
		cout << "  |   O \n";
		cout << "  |   | \n";
		cout << "  |    \n";
		cout << "  |    \n";
		cout << "========= \n";
	}
	else if (erros == 3)
	{
		cout << "  +---+ \n";
		cout << "  |   | \n";
		cout << "  |   O \n";
		cout << "  |  /| \n";
		cout << "  |    \n";
		cout << "  |    \n";
		cout << "========= \n";
	}
	else if (erros == 4)
	{
		cout << "  +---+ \n";
		cout << "  |   | \n";
		cout << "  |   O \n";
		cout << "  |  /|\\ \n";
		cout << "  |    \n";
		cout << "  |    \n";
		cout << "========= \n";
	}
	else if (erros == 5)
	{
		cout << "  +---+ \n";
		cout << "  |   | \n";
		cout << "  |   O \n";
		cout << "  |  /|\\ \n";
		cout << "  |  / \n";
		cout << "  |    \n";
		cout << "========= \n";
	}
	else if (erros == 6)
	{
		cout << "  +---+ \n";
		cout << "  |   | \n";
		cout << "  |   O \n";
		cout << "  |  /|\\ \n";
		cout << "  |  / \\ \n";
		cout << "  |    \n";
		cout << "========= \n";
	}
	
} 

void gotoxy(int x, int y)
{
	COORD coordenadas;
	coordenadas.X = x;
	coordenadas.Y = y;

	HANDLE hconsole;
	hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hconsole, coordenadas);
}
void sair() 
{
	char resposta;
	
	do
	{
	system("cls");
	gotoxy(50, 5);
	cout << "Deseja sair (S/N)?  ";
	resposta = toupper(_getch());
	} while (resposta != 'S' && resposta != 'N');
	if (resposta == 'S')
	{
		system("cls");
		gotoxy(50, 5);
		cout << "Volte sempre!" << endl;
		exit(1);
	}
}















// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
