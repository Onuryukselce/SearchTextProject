#include <iostream>
#include <cstdlib>
#include <fstream>
#include <chrono>
#include <locale.h>
#include <windows.h>
#include <string>
#include <conio.h>

using namespace std;

static string yollar[4] = {
		"TXT Veri Yolu Buraya",
		"CSV Veri Yolu Buraya",
		"HTML Veri Yolu Buraya",
		"XML Veri Yolu Buraya"
};

int static g_index = 1;

int inline minimum(int x, int y, int z) {
	return (x <= y && x <= z) ? x : (y <= x && y <= z) ? y : z;
}

struct word {
	string data;
	int index;
	word* next = NULL;
};
word* root = NULL;


int levenshtein(string s1, string s2) {

	int x, y, s1len, s2len, sonuc;
	s1len = s1.size();
	s2len = s2.size();

	int** matrix = new int* [s2len + 1];
	for (int i = 0; i < s2len + 1; i++)
		matrix[i] = new int[s1len + 1];

	matrix[0][0] = 0;

	for (x = 1; x <= s2len; x++)
		matrix[x][0] = matrix[x - 1][0] + 1;
	for (y = 1; y <= s1len; y++)
		matrix[0][y] = matrix[0][y - 1] + 1;

	for (x = 1; x <= s2len; x++)
		for (y = 1; y <= s1len; y++)
			matrix[x][y] = minimum(matrix[x - 1][y] + 1, matrix[x][y - 1] + 1, matrix[x - 1][y - 1] + (s1[y - 1] == s2[x - 1] ? 0 : 1));

	sonuc = matrix[s2len][s1len];

	for (int i = 0; i < s2len; i++)
		delete[]matrix[i];
	delete[]matrix;

	return sonuc;
}

void gecmis(string kelime) {
	ofstream gecmis("gecmis.txt", ios::app);
	gecmis << kelime << endl;
	gecmis.close();
}

void gecmisgoruntule() {

	ifstream gecmis("gecmis.txt");
	string satir = "";


	if (gecmis.is_open()) {

		getline(gecmis, satir);
		if (satir == "") {
			cout << "Dosyada kelime yok!" << endl;
		}

		while (getline(gecmis, satir)) {

			cout << satir << endl;

		}

		gecmis.close();
	}
}

void sonucgoster() {

	ifstream gecmis("sonuc.txt");
	string satir = "";


	if (gecmis.is_open()) {

		getline(gecmis, satir);
		if (satir == "") {
			cout << "Dosyada kelime yok!" << endl;
		}

		while (getline(gecmis, satir)) {

			cout << satir << endl;

		}

		gecmis.close();
	}
}

void gecmissil() {
	ofstream gecmis("gecmis.txt");
	gecmis.close();

}

word* ara(word* root, string kelime) {

	word* yakin = new word;
	word* iter = yakin;

	int size = kelime.size();
	int kriter = (size <= 5) ? 2 : 3;
	int fark = 3;

	while (root != NULL) {

		if (abs((int)(root->data.size() - kelime.size())) < fark) {

			int mesafe = levenshtein(kelime, root->data);
			if (mesafe < kriter) {

				if (mesafe == 0 && root->data.size() == kelime.size()) {
					std::cout << "tam kelime bulundu" << std::endl;
					return new word{ root->data, root->index, NULL };
				}
				iter->next = new word;
				iter->next->data = root -> data;
				iter->next->index = root -> index;
				iter = iter->next;
			}
		}
		root = root->next;
	}
	return yakin->next;
}

word* oku_xml(string path) {

	ifstream file(path, ios::in);
	string line;
	word* root = new word;
	word* iter = root;

	while (getline(file, line)) {

		if (line.find("<") == string::npos) {

			iter->data = line;
			iter->index = g_index++;

			if (!file.eof()) {
				iter->next = new word;
				iter = iter->next;
			}
		}
	}
	return root;
}

word* oku_html(string path) {

	ifstream file(path, ios::in);
	string line;
	word* root = new word;
	word* iter = root;

	while (getline(file, line)) {

		if (line.find("<") == string::npos) {

			iter->data = line;
			iter->index = g_index++;

			if (!file.eof()) {
				iter->next = new word;
				iter = iter->next;
			}
		}
	}
	return root;
}

word* oku_csv(string path) {

	ifstream file(path, ios::in);
	string line;
	word* root = new word;
	word* iter = root;

	while (getline(file, line, ',')) {

		iter->data = line;
		iter->index = g_index++;

		if (!file.eof()) {
			iter->next = new word;
			iter = iter->next;
		}
	}
	return root;
}

void sil() {
	word* iter = root;
	while (iter != NULL) {
		iter = iter->next;
		
		delete root;
		root = iter;
	}
	root = NULL;
	g_index = 1;
}

word* oku_txt(string path) {

	ifstream file(path, ios::in);
	string line;
	word* root = new word;
	word* iter = root;

	while (getline(file, line)) {

		iter->data = line;
		iter->index = g_index++;

		if (!file.eof()) {
			iter->next = new word;
			iter = iter->next;
		}
	}
	return root;
}

void yazdir(word* root, double sure, string aranilan, char sec) {
	ofstream sonuc("sonuc.txt", ios::app);
	int sayac = 0;

	if (sec == 'y')
		sonuc << "\"" << aranilan << "\"" << " kelimeye yakin bulunan degerler : " << std::endl;
	else if (sec == 't')
		sonuc << "\"" << aranilan << "\"" << " ifadesi bulundu : " << std::endl;
	else if (sec == 'b')
		sonuc << "\"" << aranilan << "\"" << " ifadesi ve yakin degerleri yok : " << std::endl;

	while (root != NULL) {
		sonuc << "kelime : " << "\"" << root->data << "\"" << "; indexi : " << root->index << endl;
		if (sec == 'y')
		{
			cout << root->data << endl;
		}
		if (sayac++ == 7)
			break;
		root = root->next;
	}
	sonuc << "Gecen sure : " << sure << " saniye" << std::endl;
	sonuc << "============================================================" << endl;
	sonuc.close();

}
void sonucsil() {
	ofstream sonuc("sonuc.txt");
	sonuc.close();

}

void arayuz(word* root) {
	char giris;
	string kelime;

	system("cls");
	cout << "Yukleme tamamlandi, yuklenen kelime sayisi: " << g_index << endl;
	while (true) {
		cout << "1 ~ Kelime aramak icin      " << endl;
		cout << "2 ~ Gecmis goruntulemek icin         " << endl;
		cout << "3 ~ Gecmis silmek icin               " << endl;
		cout << "4 ~ Dosya uzantisi degistirmek icin  " << endl;
		cout << "5 ~ Sonuc dosyasini gostermek icin  " << endl;
		cout << "6 ~ Sonuc dosyasini temizlemek icin  " << endl;
		giris = _getch();
		system("cls");
		switch (giris)
		{
			case '1':
			{
				bool cont;
				do {
					cout << "aramak istediginiz kelimeyi giriniz" << endl;
					cin >> kelime;
					gecmis(kelime);
					auto start = chrono::system_clock::now();
					word* bul = ara(root, kelime);
					auto end = chrono::system_clock::now();
					chrono::duration<double> diff = end - start;

					if (bul != NULL && (bul->next != NULL || bul->data != kelime)) {
						cout << "aradiginiz kelime yok ama en yakin degerler : " << endl;
						yazdir(bul, diff.count(), kelime, 'y');
					}
					else if (bul != NULL && bul->next == NULL) {
						cout << "Aradiginiz kelime dokuman belgesinin " << bul->index << ". satirinda var. " << endl;
						yazdir(bul, diff.count(), kelime, 't');
					}
					else {
						cout << "Aradiginiz ifade dokuman belgesinde yok ve benzer ifade de yok." << endl;
						yazdir(bul, diff.count(), kelime, 'b');
					}

					cout << "gecen zaman : " << diff.count() << "s" << endl;
					cout << "Tekrar aramak icin 1'e, geri donmek icin 2 tusuna basiniz" << endl;
					char pse;
					pse = _getch();
					switch (pse)
					{
					case '1':
						cont = true;
						break;
					case '2':
						cont = false;
						break;
					default:
						cout << "Hatali Tuslama Basa Donuldu!" << endl;
						cont = false;
						break;
					}
					system("cls");
				} while (cont == true); 
				break;
			}
			case '2':
			{
				gecmisgoruntule();
				system("pause");
				system("cls");
				break;
			}
			case '3': {
				gecmissil();
				cout << "Gecmisiniz silindi" << endl;
				system("pause");
				system("cls");
				break;
			}
			case '4': {
				char uzanti;
				cout << "Islem yapmak istediginiz dosya uzantisini seciniz" << endl;
				cout << "txt  icin  1" << endl;
				cout << "csv  icin  2" << endl;
				cout << "html icin 3" << endl;
				cout << "xml  icin 4" << endl;
				uzanti = _getch();
				system("cls");
				switch (uzanti) {

				case '1': {
					cout << " txt veritabani yukleniyor..." << endl;
					sil();
					root = oku_txt(yollar[0]);
					system("cls");
					cout << "Yukleme tamamlandi, yuklenen kelime sayisi: " << g_index << endl;

					break;
				}
				case '2': {

					cout << "csv veritabani yukleniyor..." << endl;
					sil();
					root = oku_csv(yollar[1]);
					system("cls");
					cout << "Yukleme tamamlandi, yuklenen kelime sayisi: " << g_index << endl;

					break;
				}
				case '3': {

					cout << "html veritabani yukleniyor..." << endl;
					sil();
					root = oku_html(yollar[2]);
					system("cls");
					cout << "Yukleme tamamlandi, yuklenen kelime sayisi: " << g_index << endl;
					break;
				}
				case '4': {

					cout << "xml veritabani yukleniyor..." << endl;
					sil();
					root = oku_xml(yollar[3]);
					system("cls");
					cout << "Yukleme tamamlandi, yuklenen kelime sayisi: " << g_index << endl;
					break;
				}

				default: {
					cout << "Hatali bir giris yapildi!" << endl;
					break;
				}

				}
				break;

			}
			case '5': {
				sonucgoster();
				system("pause");
				system("cls");
				break;
			}

			case '6': {

				sonucsil();
				cout << "sonuc dosyasi temizlendi.." << endl;
				system("pause");
				system("cls");
				break;
			}
			default: {

				cout << "Yanlis secim girdiniz" << endl;
				system("pause");
				system("cls");
				break;
			}
		}
	}
}

int main()
{
	cout << "txt veritabani yukleniyor..." << endl;
	root = oku_txt(yollar[0]);
	arayuz(root);
}
