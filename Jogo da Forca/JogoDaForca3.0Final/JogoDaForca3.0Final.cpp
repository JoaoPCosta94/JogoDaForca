#include <iostream>
#include <fstream>
#include <conio.h>
#include <Windows.h>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#define ESC 27
#define UP 72
#define DOWN 80
#define LEFT 75
#define ENTER 13

//======================================== Protótipos ========================================
int menu();
void umJogador(string palavra, string adivinha, string player, vector <string> NOMES, bool novoNome, vector <char> ESCONDIDAS, vector <char> TENTATIVAS, int erros, bool ganhou, bool acertou, string input, char letra, int pontuacao);
void doisJogadores(string player, vector <string> NOMES, bool novoNome, string adivinha, vector <char>& ESCONDIDAS, int erros, bool ganhou, bool acertou, vector <char> TENTATIVAS, string input, char letra, int pontuacao);
void randPalavra(string palavra, vector <string>& PALAVRAS, int& indice, string& adivinha);
void escondePalavra(string adivinha, vector <char>& ESCONDIDAS);
void verificarJogador(string& player, vector <string>& NOMES, bool& novoNome);
void inputJogador(string adivinha, string& input, char& letra, vector <char>& ESCONDIDAS, bool& acertou, int& erros, vector <char>& TENTATIVAS, bool& ganhou);
void verificarVitoria(vector <char> ESCONDIDAS, bool& ganhou);
void desenharBoneco(int erros);
void Pontuacao(string& player, int& pontuacao);
void Leaderboard();
void gotoxy(int x, int y);
void sair();

int main()
{
	SetConsoleOutputCP(1252);

	string adivinha;				 		 // Vai armazenar a palavra a adivinhar
	vector <string> NOMES;					 // Nomes dos jogadores para podermos controlar os pontos
	vector <string> letrasErradas;           // Armazena as tentativas erradas
	vector <char> ESCONDIDAS;				 // Vai armazenar os "_" e substituir pela letra certa se o jogador adivinhar
	vector <char> TENTATIVAS;				 // Para mostrar as letras erradas que o jogador usou
	string palavra;
	string player;
	string input;							 // Caso o jogador tente adivinhar logo a palavra
	char letra = 0;								 // Variavel para o input da letra
	bool acertou = false;
	bool novoNome = true;
	bool ganhou = false;
	int erros = 0;
	int pontuacao = 0;

	gotoxy(50, 10);
	while (true)
	{
		int escolha = menu();

		switch (escolha)
		{
		case 0: umJogador(palavra, adivinha, player, NOMES, novoNome, ESCONDIDAS, TENTATIVAS, erros, ganhou, acertou, input, letra, pontuacao);  break;
		case 1: doisJogadores(player, NOMES, novoNome, adivinha, ESCONDIDAS, erros, ganhou, acertou, TENTATIVAS, input, letra, pontuacao); break;
		case 2: Leaderboard(); break;
		case 3: sair(); break;
		}
	}
}
//==============================================================================================================
//Menu
int menu() {
	int setas = 0;
	int escolha = 0;
	string OP[4] = { "Um Jogador", "Dois Jogadores", "Leaderboard", "Sair" };
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
				cout << "   " << OP[i];   // precisa de um espaço para alinhar porque a escolha da opçao empurra o texto
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
void umJogador(string palavra, string adivinha, string player, vector <string> NOMES, bool novoNome, vector <char> ESCONDIDAS, vector <char> TENTATIVAS, int erros, bool ganhou, bool acertou, string input, char letra, int pontuacao)
{
	int indice = 0;							 // Indice para armazenar o numero aleatorio do rand()
	vector <string> PALAVRAS;				 // 'Pool' de onde vamos tirar uma palavra aleatoria

	ESCONDIDAS = {};									// dar reset ao vector, importante se vier do dois jogadores para o primeiro
	TENTATIVAS = {};									

	randPalavra(palavra, PALAVRAS, indice, adivinha);

	verificarJogador(player, NOMES, novoNome);

	system("cls");
	gotoxy(50, 5);

	cout << "Bem-vindo(a) " << player << "!";

	Sleep(3000);
	system("cls");

	escondePalavra(adivinha, ESCONDIDAS);

	while (erros < 7 && ganhou == false)
	{
		system("cls");
		acertou = false;
		desenharBoneco(erros);
		cout << endl;
		for (char letra : ESCONDIDAS) {
			cout << letra << " ";
		}
		cout << endl << endl;
		cout << "ERRADAS: ";
		for (char letra : TENTATIVAS) {
			cout << letra << " ";
		}
		cout << endl;

		inputJogador(adivinha, input, letra, ESCONDIDAS, acertou, erros, TENTATIVAS, ganhou);
		verificarVitoria(ESCONDIDAS, ganhou);

		if (ganhou == true)
		{
			pontuacao += 10;
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
			Pontuacao(player, pontuacao);
			break;
		}
	}
	if (ganhou == false)
	{
		system("cls");
		gotoxy(50, 5);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		cout << "Perdeu!";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		Sleep(3000);
	}


}
//==============================================================================================================
// Função do jogo da forca para dois jogadores
void doisJogadores(string player, vector <string> NOMES, bool novoNome, string adivinha, vector <char>& ESCONDIDAS, int erros, bool ganhou, bool acertou, vector <char> TENTATIVAS, string input, char letra, int pontuacao)
{
	string aux;				//para trocar o jogador1 com o jogador2 para nao ter que tar a reescrever codigo
	char resposta;
	string jogador1;
	string jogador2;
	system("cls");
	gotoxy(50, 5);
	cout << "Jogador 1";
	Sleep(1500);
	verificarJogador(player, NOMES, novoNome);
	jogador1 = player;
	system("cls");
	gotoxy(50, 5);
	cout << "Jogador 2";
	Sleep(1500);
	verificarJogador(player, NOMES, novoNome);
	jogador2 = player;
	system("cls");
	gotoxy(50, 5);
	cout << "Bem vindos " << jogador1 << " e " << jogador2 << "!";
	Sleep(2000);
	system("cls");
	do
	{
		for (int i = 0; i < 3; i++)
		{
			system("cls");
			gotoxy(50, 5);
			cout << jogador1 << " escolha uma palavra: ";
			cin >> adivinha;
			cin.clear();
			cin.ignore(100, '\n');
			for (int i = 0; i < adivinha.size(); i++)			//Transformar a palavra em maiusculas
			{
				adivinha[i] = toupper(adivinha[i]);
			}
			ESCONDIDAS = {};									// dar reset ao vector
			TENTATIVAS = {};
			escondePalavra(adivinha, ESCONDIDAS);
			ganhou = false;										// dar reset para poder entrar no loop outra vez
			erros = 0;
			while (erros < 7 && ganhou == false)
			{
				system("cls");
				acertou = false;
				desenharBoneco(erros);
				cout << endl;
				for (char letra : ESCONDIDAS) {
					cout << letra << " ";
				}
				cout << endl << endl;
				cout << "ERRADAS: ";
				for (char letra : TENTATIVAS) {
					cout << letra << " ";
				}
				cout << endl;

				inputJogador(adivinha, input, letra, ESCONDIDAS, acertou, erros, TENTATIVAS, ganhou);
				verificarVitoria(ESCONDIDAS, ganhou);

				if (ganhou == true)
				{
					pontuacao += 10;
					system("cls");
					gotoxy(50, 5);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
					cout << "Parabéns " << jogador2 << " ganhou!";
					gotoxy(50, 6);
					cout << "A palavra era ";
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
					cout << adivinha;
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
					Sleep(3000);
					Pontuacao(jogador2, pontuacao);
					pontuacao = 0;						//dar reset à pontuaçao
					break;
				}
			}
			if (ganhou == false)
			{
				system("cls");
				gotoxy(50, 5);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
				cout << "Perdeu!";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				Sleep(3000);
			}
			aux = jogador1;
			jogador1 = jogador2;
			jogador2 = aux;
			i++;
		}
		system("cls");
		gotoxy(50, 5);
		cout << "Replay? (S/N) ";
		resposta = toupper(_getche());
		if (resposta == 'S')
		{
			jogador2 = jogador1;
			aux = jogador1;
		}

	} while (resposta == 'S');




}
//==============================================================================================================
// Função para escolher uma palavra aleatória
void randPalavra(string palavra, vector <string>& PALAVRAS, int& indice, string& adivinha) {

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
void escondePalavra(string adivinha, vector <char>& ESCONDIDAS)
{
	for (int i = 0; i < adivinha.size(); i++)
	{
		ESCONDIDAS.push_back('_');					// Guardar um vector com '_' que vai dizer a quantidade de letras escondidas que a palavra a adivinhar tem
	}

}
//==============================================================================================================
// Função para verificar jogador
void verificarJogador(string& player, vector <string>& NOMES, bool& novoNome)
{
	system("cls");
	gotoxy(50, 5);
	ifstream fread;
	fread.open("nomes.txt");

	while (getline(fread, player))						// Preencher o vector com os nomes
	{
		NOMES.push_back(player);
	}

	cout << "Nome: ";									// Pedir o nome ao jogador

	getline(cin, player);

	for (int i = 0; i < NOMES.size(); i++)				// Verificar se é um jogador novo e acrescentar à lista se assim for
	{
		if (player == NOMES[i])
		{
			novoNome = false;
			break;
		}
	}
	if (novoNome == true)
	{
		NOMES.push_back(player);
		ofstream fwrite;
		fwrite.open("nomes.txt", ios::app);			//ios::app para acrescentar ao ficheiro, porque senao substitui o que la está pelo que vou escrever
		fwrite << "\n" << player;
		fwrite.close();
	}
	fread.close();
}
//==============================================================================================================
// Função para introduzir o input do jogador
void inputJogador(string adivinha, string& input, char& letra, vector <char>& ESCONDIDAS, bool& acertou, int& erros, vector <char>& TENTATIVAS, bool& ganhou)
{
	acertou = false;
	cout << endl;
	cout << "Escolha uma letra (ou tente adivinhar a palavra):  ";
	getline(cin, input);

	for (int i = 0; i < input.size(); i++) {				// Para converter o input em maiusculas, porque eu tenho as palavras armazenadas no txt em maiusculas
		input[i] = toupper(input[i]);
	}

	if (input.size() == 1)
	{
		letra = input[0];
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

	}
	else
	{
		if (input == adivinha)
		{
			ganhou = true;
			// tive problemas com as respostas e a posição da funçao verificarVitoria, ou aceitava a palavra e nao ganhava logo quando metia a ultima letra
			// ou ganhava logo quando metia a ultima letra e nao aceitava a palavra, com este 'for' resolvi esse problema.
			for (int i = 0; i < adivinha.size(); i++)
			{
				ESCONDIDAS[i] = adivinha[i];
			}
		}
		else
		{
			cout << "Palavra errada!" << endl;
			erros++;
			Sleep(1000);
		}
	}


}
//==============================================================================================================
// Função para verificar se ganhou
void verificarVitoria(vector <char> ESCONDIDAS, bool& ganhou)
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
//==============================================================================================================
// Função para a pontuação  // para ser sincero tive de pedir ajuda ao chatgpt para esta e para a leaderboard porque ja estava a b
void Pontuacao(string &player, int &pontuacao)
{
	fstream freadNames("nomes.txt");
	ifstream freadScores("scores.txt");

	vector <string> nomes;
	vector <int> scores;
	string nome;
	int score;

	
	while (getline(freadNames, nome) && freadScores >> score)				// Ler os nomes e as pontuações existentes
	{					
		nomes.push_back(nome);
		scores.push_back(score);
	}

	freadNames.close();
	freadScores.close();

	bool jogadorExistente = false;
	for (int i = 0; i < nomes.size(); ++i)									// Verificar se o jogador existe na lista
	{
		if (nomes[i] == player) {
			scores[i] += pontuacao;											// Atualiza a pontuação do jogador
			jogadorExistente = true;
			break;
		}
	}

	
	if (!jogadorExistente)													// Se o jogador não existir, adicionar ao final das listas
	{
		nomes.push_back(player);
		scores.push_back(pontuacao);
	}

	// Gravar as atualizações nos arquivos
	ofstream fwriteNames("nomes.txt");
	ofstream fwriteScores("scores.txt");

	for (int i = 0; i < nomes.size(); ++i) {
		fwriteNames << nomes[i] << endl;
		fwriteScores << scores[i] << endl;
	}

	fwriteNames.close();
	fwriteScores.close();

}
//==============================================================================================================
// Função para mostrar a leaderboard 
void Leaderboard()
{
	system("cls");
	ifstream freadNames("nomes.txt");
	ifstream freadScores("scores.txt");

	vector<pair<string, int>> leaderboard;
	string nome;
	int score;


	while (getline(freadNames, nome) && freadScores >> score)				// Ler os nomes e pontuações e armazenar como pares em um vetor
	{					
		leaderboard.push_back(make_pair(nome, score));
	}

	freadNames.close();
	freadScores.close();

	
	sort(leaderboard.begin(), leaderboard.end(), [](const pair<string, int>& a, const pair<string, int>& b)		// Ordenar o vetor pela pontuação em ordem decrescente
		{
		return a.second > b.second;  // Comparação pela pontuação (second)
		});

	// Exibir o leaderboard
	gotoxy(50, 5);
	cout << "===== Leaderboard =====" << endl;
	for (int i = 0; i < leaderboard.size(); ++i) 
	{
		gotoxy(50, 6+i);
		cout << i + 1 << ". " << leaderboard[i].first << " - " << leaderboard[i].second << " pontos" << endl;
	}
	gotoxy(30, 10);
	cout << "\n\n\n\nPressione um botao para retornar ao menu!";
	_getche();
}
//==============================================================================================================
// Função para posicionar o cursor nas coordenadas que quiser
void gotoxy(int x, int y)
{
	COORD coordenadas;
	coordenadas.X = x;
	coordenadas.Y = y;

	HANDLE hconsole;
	hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hconsole, coordenadas);
}
//==============================================================================================================
// Função para sair do programa
void sair()
{
	char resposta;

	do
	{
		system("cls");
		gotoxy(50, 5);
		cout << "Deseja sair (S/N)?  ";
		resposta = toupper(_getche());
	} while (resposta != 'S' && resposta != 'N');
	if (resposta == 'S')
	{
		system("cls");
		gotoxy(50, 5);
		cout << "Volte sempre!" << endl;
		exit(1);
	}
}