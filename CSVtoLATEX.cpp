#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int sloupec = 0, radky = 0;

void uvodTex(ofstream &f, char volba)//neplati pro vlastni volbu
{
	char mezera  = ' ';
	f << "\\begin{table}[htbp]" << endl;
	f << "\\centering" << endl;
	f << "\t\\begin{tabular}{";
	if (volba == 'f')//fyzikalni tabulka
	{
		f << "|";
		for (int i = 0; i <= radky; i++)
		{
			f << "c";
		}
		f << "|";
	}
	else if (volba == 'e')//excel
	{
		f << '|';
		for (int i = 0; i <= radky; i++)
		{
			f << "c|";
		}
	}
	else if (volba == 'b')//bez car
	{
		for (int i = 0; i <= radky; i++)
		{
			f << "c";
		}
	}
	f << "}" << endl;
}

void endTex(ofstream &f)
{
	f << "\t\\end{tabular}" << endl;
	f << "\\end{table}" << endl;
}
void getLengthOfLine(string cesta, char oddelovac, char radek)
{
	int max = 0, count = 0, sloupce = 0;
	char znak;
	bool pom=false;
	ifstream f(cesta);
	if (f.is_open())
	{
		while (!f.eof())
		{
			f.get(znak);
			while (znak != radek && !f.eof())//radek
			{
				if (znak == oddelovac)//oddelovac
					count++;
				f.get(znak);
				//cout << znak<<" h";
				pom=true;
			}
			//cout<<"-"<<znak<<"-";
			if (max<count)
				max = count;
			//if ((znak == radek || f.eof()))
				sloupce++;
				//cout<<"novy radek";
			count = 0;
		}
		pom=false;
	}
	f.close();
	//cout << "Tabulka je RxS: " << max+1 << " x " << sloupce << endl;
	sloupec = sloupce;
	radky = max;
	//vratit se na novy radek
}

void physTable(ifstream &f, ofstream &latex, char radek,char oddelovac,char desetinaCarka, char novaCarka)
{
	char znak = ' ';
	int rad, sloup = 0;
	latex << "\\hline ";
	if (f.is_open() && latex.is_open())
	{
		//cout << "Soubor je otevren!" << endl;
		while (!f.eof())
		{
			f.get(znak);
			rad = 0;
			while (!f.eof() && znak != radek )
			{
				if (znak == oddelovac)//oddeleni zaznamu
				{
					rad++;
					latex << " & ";
				}
				else if (znak == desetinaCarka)//desetnia carka
				{
					latex << novaCarka;//nova desetina carka
				}
				else if (znak != radek)//jedna se o zaznam
				{
					latex << znak;
				}
				f.get(znak);
				//cout<<znak<<endl;
			}
			for (int i = 0; i<(radky - rad); i++)
			{
				latex << " & ";
			}
			//rewind(latex);
			if (sloup == 0)
				latex << "\\\\ \\hline \\hline " << endl;
			else if (sloup == sloupec - 1)
				latex << "\\\\ \\hline " << endl;
			else
				latex << "\\\\"<<endl;
			sloup++;
		}
	}
	f.close();
}
void excelTable(ifstream &f, ofstream &latex, char radek,char oddelovac,char desetinaCarka, char novaCarka)
{
	char znak = ' ';
	int rad;
	latex << "\\hline ";
	if (f.is_open() && latex.is_open())
	{
		//cout << "Soubor je otevren!" << endl;
		while (!f.eof())
		{
			f.get(znak);
			rad = 0;
			while (znak != radek && !f.eof())
			{
				if (znak == oddelovac)//konec sloupce 
				{
					rad++;
					latex << " & ";
				}
				else if (znak == desetinaCarka)//desetnia carka
				{
					latex << novaCarka;
				}
				else if (znak != radek)//jedna se o zaznam
				{
					latex << znak;
				}
				f.get(znak);
				//cout<<znak<<endl;
			}
			for (int i = 0; i<(radky - rad); i++)
			{
				latex << " & ";
			}
			//rewind(latex);
			latex << "\\\\ \\hline " << endl;

		}
	}
	f.close();
}
void withoutLines(ifstream &f,ofstream &latex, char radek,char oddelovac,char desetinaCarka, char novaCarka)
{
	char znak = ' ';
	int rad;
	if (f.is_open() && latex.is_open())
	{
		//cout << "Soubor je otevren!" << endl;
		while (!f.eof())
		{
			f.get(znak);
			rad = 0;
			while (znak != radek && !f.eof())
			{
				if (znak == oddelovac)//konec sloupce 
				{
					rad++;
					latex << " & ";
				}
				else if (znak == desetinaCarka)//desetnia carka
				{
					latex << novaCarka;
				}
				else if (znak != radek)//jedna se o zaznam
				{
					latex << znak;
				}
				f.get(znak);
				//cout<<znak<<endl;
			}
			for (int i = 0; i<(radky - rad); i++)
			{
				latex << " & ";
			}
			//rewind(latex);
			latex << "\\\\" << endl;

		}
	}
	f.close();
}

char getCharacter ()
{
	string pom;
	char out;
	getline(cin,pom);
	out=pom[0];
	return out;
}

int main(int argc,char* argv[])
{
	string cesta, outcesta;
	char znak, odpoved, typ, carka = '.', novaCarka='.', oddelovac = ',', radek = '\n';
	int rad = 0;
	bool spravnaOdpoved=true;

	if (argc==2 && string(*(argv+1))=="help" )
	{
		cout<<"CSVtoLatex version 1.0"<<endl<<"Examples of using this program: "<<endl<<
			"\tCSVtoLatex input output type - f for physics table, e for excel, b for table wihtou any lines"<<endl<<
			"\tCSVtoLatex - this wil intiate TUI."<<endl;
	}
	else if(argc==4  )
	{
		char a=**(argv+3);
		cesta=string(*(argv+1))+".csv";
		outcesta=string(*(argv+2))+".tex";
		ifstream f(cesta);
		ofstream latex(outcesta);
		getLengthOfLine(cesta, ',', '\n');
		if (f.good() && f.is_open() && latex.is_open())
		{
				switch (a)
				{
				case 'f':
					uvodTex(latex, 'f');
					physTable(f, latex, radek,oddelovac,carka,novaCarka); 
					spravnaOdpoved=false; break;
				case 'e':
					uvodTex(latex, 'e');
					excelTable(f, latex, radek,oddelovac,carka,novaCarka); 
					spravnaOdpoved=false; break;
				case 'b':
					uvodTex(latex, 'b');
					withoutLines(f, latex,radek,oddelovac,carka,novaCarka); 
					spravnaOdpoved=false; break;
				default:
					cout << "Zadali jste spatnou volbu!" << endl; 
					spravnaOdpoved=true; break;
				}	
			endTex(latex);
			latex.close();
		}
		else
		{
			cout<<"Error: there is no file with this name!"<<endl;
		}
	}
	else
	{
		cesta = "tabulka.csv";
		outcesta = "tabulka.tex";

		cout << "----------------Vitejte v programu csvToLatex!------------------" << endl;
		cout << "ZVOLTE NAZEV SOUBORU" << endl;
		/*while (spravnaOdpoved)
		{
			cout << "Zadejte nazev vstupniho souboru: " << endl;
			getline(cin, cesta);
			cesta += ".csv";
			fstream tryf(cesta);
			if(tryf.good())
			{
				tryf.close();
				spravnaOdpoved=false;
			}
			else
				cout<<"Chyba takovy soubor neexistuje!"<<endl;	
		}
		spravnaOdpoved==true;*/

		cout << "Zadejte nazev vstupniho souboru: " << endl;
		getline(cin, cesta);
		cout << "Zadejte nazev vystupniho souboru" << endl;
		getline(cin, outcesta);
	
		cesta += ".csv";
		outcesta += ".tex";

		cout << "ZVOLTE TYP CSV" << endl << "Pro standartni nastaveni stisknete - s " << endl << "Pro vlastni nastaveni stisknete - v" << endl;
		typ = getCharacter();
		//getchar();

		if (typ == 'v')
		{
			cout << "Zadejte oddelovac desetine carky: " << endl;
			carka = getCharacter();
			cout << "Nahrazeni desetine carky: " << endl;
			novaCarka = getCharacter();
			cout << "Zadejete odelovac udaju: " << endl;
			oddelovac = getCharacter();
			cout << "Zadejte oddelovac noveho radku(pro \\n zadejte 'n'): " << endl;
			radek = getCharacter();
			if (radek=='n')
				radek='\n';
			//cout << carka << endl;
		}	

		//nejdrive najdeme pocet sloupcu
		getLengthOfLine(cesta, oddelovac, radek);
		//Pote zascneme sekvence ukladat
		ifstream f(cesta);
		ofstream latex(outcesta);
		if (f.is_open()==true)
		{
			cout << endl << "ZVOLTE TYP TABULKY" << endl;
			cout << "Fyzikalni tabulka - f" << endl << "Excelovska tabulka - e" << endl << "Bez car - b" << endl;
			while(spravnaOdpoved)
			{
				odpoved = getchar();
				switch (odpoved)
				{

				case 'f':
					cout<<"Soubor je otevren!"<<endl;
					uvodTex(latex, 'f');
					physTable(f, latex, radek,oddelovac,carka,novaCarka); 
					spravnaOdpoved=false; break;
				case 'e':
					cout<<"Soubor je otevren!"<<endl;
					uvodTex(latex, 'e');
					excelTable(f, latex, radek,oddelovac,carka,novaCarka); 
					spravnaOdpoved=false; break;
				case 'b':
					cout<<"Soubor je otevren!"<<endl;
					uvodTex(latex, 'b');
					withoutLines(f, latex, radek,oddelovac,carka,novaCarka); 
					spravnaOdpoved=false; break;
				default:
					cout << "Zadali jste spatnou volbu!" << endl; 
					spravnaOdpoved=true; break;
				}	
			}
			

			endTex(latex);
			latex.close();
			cout << "Soubor byl uspesne nacten." << endl;
			cout << "----------------Tabulka uspesne prevedena------------------------" << endl;	
		}
	}
	return 0;
}