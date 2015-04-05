////////////////////////////////////////////////////////////////////////
// CPucePuce.cpp                                                      //
// Lecteur de carte � puce du type T1G et T2G.                        //
//                                                                    //
// Version: 1.0                                                       //
// Auteur : Werner BEROUX                                             //
// Site   : www.alc-wbc.com                                           //
// E-mail : wbc@alc-wbc.com                                           //
//                                                                    //
////////////////////////////////////////////////////////////////////////


// Headers
#include <windows.h>													// Windows Header Files:
#include <stdlib.h>														// C RunTime Header Files
#include <malloc.h>														// C RunTime Header Files
#include <memory.h>														// C RunTime Header Files
#include <tchar.h>														// C RunTime Header Files
#include <stdio.h>														// Le fonction sprintf(...)
#include <conio.h>														// Fonctions de lecture du port
#include "resource.h"													// Notre fichier ressource

// Variables globales
HWND	g_hDlg;															// Handle vers notre boite de dialogue

// Prototypes
LRESULT CALLBACK	DialogProc(HWND, UINT, WPARAM, LPARAM);				// Traitement des message de la boite de dialogue principale
void				LirePuce();											// Lis les information de la carte � puce
int					NombreDeUn(bool *bData, int debut, int fin);		// Retourne le nombre de 1 entre debut et fin dans bData
void				GetPort(unsigned short &sBasePort);					// R�cup�re l'adresse du port parall�le
void				GetPortAndBit(int nIDDlgItem, unsigned short &sPort, unsigned char &cBit);	// R�cup�re l'item s�lectionn� et donne l'adresse du port et le bit � modifier pour y acc�der


////////////////////////////////////////////////////////////////////////
// Fonctions

// Le programme commence ici.
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	// On cr�� la boit de dialogue
	DialogBox(hInstance, (LPCTSTR)IDD_MAIN, NULL, (DLGPROC)DialogProc);

	return false;
}

//  Traite les message de la fen�tre.
LRESULT CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND	hWndCtrl;
	int		wmId,
			wmEvent;

	switch (message) 
	{
	case WM_INITDIALOG:													// Initialisation
		g_hDlg = hDlg;													// Au d�finis la variable globale
		// LPT
		hWndCtrl = GetDlgItem(hDlg, IDC_LPT_COMBO);
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "0x378");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "0x278");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "0x3BC");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "0x2BC");
		SendMessage(hWndCtrl, CB_SETCURSEL, 0, 0);
		// D�tection d'insertion
		hWndCtrl = GetDlgItem(hDlg, IDC_DETEC_COMBO);
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "Aucun");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/ERROR");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "SELECT");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "PE");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/ACK");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "BUSY");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/STROBE");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/AUTOFEED");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/INIT");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/SELECT IN");
		SendMessage(hWndCtrl, CB_SETCURSEL, 0, 0);
		// Vcc
		hWndCtrl = GetDlgItem(hDlg, IDC_VCC_COMBO);
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "Aucun");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D0");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D1");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D2");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D3");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D4");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D5");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D6");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D7");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/STROBE");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/AUTOFEED");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/INIT");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/SELECT IN");
		SendMessage(hWndCtrl, CB_SETCURSEL, 5, 0);
		// Read/Write
		hWndCtrl = GetDlgItem(hDlg, IDC_RW_COMBO);
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "Aucun");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D0");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D1");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D2");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D3");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D4");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D5");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D6");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D7");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/STROBE");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/AUTOFEED");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/INIT");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/SELECT IN");
		SendMessage(hWndCtrl, CB_SETCURSEL, 2, 0);
		// Horloge
		hWndCtrl = GetDlgItem(hDlg, IDC_CLK_COMBO);
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D0");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D1");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D2");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D3");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D4");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D5");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D6");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D7");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/STROBE");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/AUTOFEED");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/INIT");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/SELECT IN");
		SendMessage(hWndCtrl, CB_SETCURSEL, 2, 0);
		// Reset/Up
		hWndCtrl = GetDlgItem(hDlg, IDC_RESET_COMBO);
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "Aucun");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D0");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D1");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D2");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D3");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D4");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D5");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D6");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D7");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/STROBE");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/AUTOFEED");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/INIT");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/SELECT IN");
		SendMessage(hWndCtrl, CB_SETCURSEL, 4, 0);
		// Vpp
		hWndCtrl = GetDlgItem(hDlg, IDC_VPP_COMBO);
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "Aucun");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D0");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D1");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D2");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D3");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D4");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D5");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D6");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "D7");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/STROBE");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/AUTOFEED");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/INIT");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/SELECT IN");
		SendMessage(hWndCtrl, CB_SETCURSEL, 5, 0);
		// Entr�e/Sortie
		hWndCtrl = GetDlgItem(hDlg, IDC_ES_COMBO);
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/ERROR");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "SELECT");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "PE");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/ACK");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "BUSY");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/STROBE");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/AUTOFEED");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/INIT");
		SendMessage(hWndCtrl, CB_ADDSTRING, 0, (LONG) (LPSTR) "/SELECT IN");
		SendMessage(hWndCtrl, CB_SETCURSEL, 4, 0);
		break;

	case WM_COMMAND:													// Traitre les commandes
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		switch (wmId)
		{
		case IDCANCEL:													// Quand on clique sur "Quitter"
			PostQuitMessage(0);											// Quitte l'application
			break;
		case IDOK:														// Quand on clique sur "Lire"
			LirePuce();													// Lance la lecture
			break;
		}
		break;

	case WM_DESTROY:													// On quitte tout
		PostQuitMessage(0);
		break;
   }
   return 0;
}

// Lis les donn�es de la puce
void LirePuce()
{
	char			lzDataText[600];									// Donn�e sous forme de texte (512 de donn�es + les espaces + les retour de chariot + le zero final + un peu d'espace vide :)
	bool			bData[512],											// 512 bits (maxi)
					bT2G;												// Version de la carte.
	unsigned short	sPortDonnees,										// Addresse du port de donn�es
					sPortInsertion,										// D�tection d'insertion
					sPortVcc,											// Vcc
					sPortRW,											// R/W
					sPortClk,											// Horloge
					sPortReset,											// Reset
					sPortVpp,											// Vpp
					sPortES;											// E/S
	unsigned char	cBitInsertion,										// Le bit � modifier
					cBitVcc,											// ...
					cBitRW,												// ...
					cBitClk,											// ...
					cBitReset,											// ...
					cBitVpp,											// ...
					cBitES;												// ...
	int				i,													// Variable g�n�rique pour les boucles
					nDataText,											// Index pour lzDataText
					nBits,												// Nombre de bits dans le bloc actuel
					nOctets,											// Nombre d'octets sur la ligne actuelle
					nUnitTotal,											// Nombre total d'unit�es
					nUnit,												// Nombre d'unit�es restantes
					nPuissance;											// Utilis� pour le calcul dans un base donn�e

	// Lis la configuration des liens LPT/Puce
	GetPortAndBit(IDC_DETEC_COMBO,	sPortInsertion,	cBitInsertion);
	GetPortAndBit(IDC_VCC_COMBO,	sPortVcc,		cBitVcc);
	GetPortAndBit(IDC_RW_COMBO,		sPortRW,		cBitRW);
	GetPortAndBit(IDC_CLK_COMBO,	sPortClk,		cBitClk);
	GetPortAndBit(IDC_RESET_COMBO,	sPortReset,		cBitReset);
	GetPortAndBit(IDC_VPP_COMBO,	sPortVpp,		cBitVpp);
	GetPortAndBit(IDC_ES_COMBO,		sPortES,		cBitES);

	// Initiatise
	GetPort(sPortDonnees);
	_outp(sPortDonnees, 0x00);											// Tout � 0

	// Alimentation
	if (sPortVcc)
		_outp(sPortVcc, _inp(sPortVcc) | cBitVcc);						// Vcc 1   -> Alimentation +5V
	if (sPortVpp)
		_outp(sPortVpp, _inp(sPortVpp) | cBitVpp);						// Vpp 1   -> Alimentation +5V (+21V pour �crire)
	Sleep(20);

	// Reset le pointeur
	_outp(sPortClk, _inp(sPortClk) | cBitClk);							// Clock 1 -> Reset pointer
	_outp(sPortClk, _inp(sPortClk) & ~cBitClk);							// Clock 0
	_outp(sPortReset, _inp(sPortReset) | cBitReset);					// Reset 1 -> Au prochain clock up, d�place le pointeur

	// Lis toutes les donn�es
	nBits = 0;
	nOctets = 0;
	nDataText = 0;
	for (i=0; i<512; i++)
	{
		// Lis les donn�es
		if (_inp(sPortES) & cBitES)										// Lis les donn�es depuis le port et bit d�finis
		{
			lzDataText[nDataText++] = '0';
			bData[i] = false;
		}
		else
		{
			lzDataText[nDataText++] = '1';
			bData[i] = true;
		}

		// Met un peu en forme les donn�es
		nBits++;														// Nombre de bits dans le bloc actuel
		if (nBits == 8)													// 8 bits par bloc
		{
			nBits = 0;													// Nouveau bloc, avec donc 0 bits
			nOctets++;													// Un octet de plus
			lzDataText[nDataText++] = ' ';								// Ajoute un espace
		}
		if (nOctets == 8)												// 8 octects par ligne
		{
			nOctets = 0;												// Nouvelle ligne, avec donc 0 octects
			lzDataText[nDataText++] = '\r';								// Place un saut de line (1)
			lzDataText[nDataText++] = '\n';								// Place un saut de line (2)
		}

		// D�place le pointeur
		_outp(sPortClk, _inp(sPortClk) | cBitClk);						// Clock 1 -> D�place le pointeur
		_outp(sPortClk, _inp(sPortClk) & ~cBitClk);						// Clock 0
	}

	// On met tout � 0 (alimentation de la puce etc.)
	_outp(sPortDonnees, 0x00);											// Tout � 0

	// Test la version T2G ou T1G
	for (i=0; i<256; i++)												// Pour 256 bits
		if (bData[i] != bData[i+256])									// Si les 256 permiers sont �quivalents aux 256 dernier: c la version 1
			goto T2G;
	bT2G = false;
T2G:

	// Place le caract�re de fin de cha�ne
	if (bT2G)
		lzDataText[nDataText] = '\0';									// On ferme la cha�ne.
	else
		lzDataText[nDataText/2] = '\0';									// On ferme la cha�ne.

	// Affiche les donn�es brutes
	SetDlgItemText(g_hDlg, IDC_DATA_EDIT, lzDataText);

	// Version
	if (bT2G)
		SetDlgItemText(g_hDlg, IDC_VERSION_EDIT, "T2G");
	else
		SetDlgItemText(g_hDlg, IDC_VERSION_EDIT, "T1G");

	// Unit�es totales
	nUnitTotal = 0;
	if (bT2G)
	{
		// T2G
		nPuissance = 1;
		for (i=63; i>=52; i--)											// Convertis les bits 52 � 64 de binaire � d�cimal
		{
			if (bData[i])
				nUnitTotal += nPuissance;
			nPuissance *= 2;
		}
		nUnitTotal *= 10;												// Multiplis par 10
	}
	else
	{
		// T1G
		nPuissance = 1;
		for (i=95; i>=89; i--)											// Convertis les bits 52 � 64 de binaire � d�cimal
		{
			if (bData[i])
				nUnitTotal += nPuissance;
			nPuissance *= 2;
		}
		nUnitTotal = nUnitTotal*10 - 10;
	}
	sprintf(lzDataText, "%d", nUnitTotal);								// Met en cha�ne la valeur enti�re
	SetDlgItemText(g_hDlg, IDC_TOTAL_EDIT, lzDataText);					// Affiche

	// Unit�es restants
	if (bT2G)
	{
		// T2G
		nUnit = nUnitTotal + 9;
		nPuissance = 1;
		for (i=11*8; i>=8*8; i-=8)										// Convertis les octets (dont on prend le nombre de 1) en octal en d�cimal
		{
			nUnit -= NombreDeUn(bData, i, i+8) * nPuissance;
			nPuissance *= 8;
		}
	}
	else
	{
		// T1G
		nUnit = nUnitTotal + 10;
		for (i=96; i<252; i++)											// Pour tous les bits concerant les unit�es consomm�es
		{
			if (!bData[i])												// Au premier 0 on arr�te
				break;
			nUnit--;													// Allez une unit�e en moins.
		}
	}
	sprintf(lzDataText, "%d", nUnit);									// Met en cha�ne la valeur enti�re
	SetDlgItemText(g_hDlg, IDC_RESTE_EDIT, lzDataText);					// Affiche
}

// Retourne le nombre de 1 entre debut et fin dans bData
int NombreDeUn(bool *bData, int debut, int fin)
{
	int i, n;
	n = 0;
	for (i=debut; i<fin; i++)
		if (bData[i])
			n++;
	return n;
}

// R�cup�re l'adresse du port parall�le
void GetPort(unsigned short &sBasePort)
{
	HWND	hDlgItem;

	// R�cup�re le HWND
	hDlgItem = GetDlgItem(g_hDlg, IDC_LPT_COMBO);
	
	// Interpr�te la s�lection en cours
	switch (SendMessage(hDlgItem, CB_GETCURSEL, 0, 0))
	{
	case 0:
		sBasePort = 0x378;
		break;
	case 1:
		sBasePort = 0x278;
		break;
	case 2:
		sBasePort = 0x3BC;
		break;
	case 3:
		sBasePort = 0x2BC;
		break;
	}
}

// R�cup�re l'item s�lectionn� et donne l'adresse du port et le bit � modifier pour y acc�der
void GetPortAndBit(int nIDDlgItem, unsigned short &sPort, unsigned char &cBit)
{
	char			lzBuf[32];
	unsigned short	sBasePort;
	HWND			hDlgItem;

	// R�cup�re le port de base
	GetPort(sBasePort);
	
	// R�cup�re le HWND
	hDlgItem = GetDlgItem(g_hDlg, nIDDlgItem);

	// R�cup�re le texte de l'item s�lectionn�
	if (SendMessage(hDlgItem, WM_GETTEXT, sizeof(lzBuf), (LPARAM) lzBuf) == 0)
	{
		sPort = 0;
		cBit = 0;
		return;
	}

	// Interpr�te la s�lection en cours
	// Donn�es
	if (stricmp(lzBuf, "D0") == 0)
	{
		sPort = sBasePort;
		cBit = 1;
	}
	else if (stricmp(lzBuf, "D1") == 0)
	{
		sPort = sBasePort;
		cBit = 2;
	}
	else if (stricmp(lzBuf, "D2") == 0)
	{
		sPort = sBasePort;
		cBit = 4;
	}
	else if (stricmp(lzBuf, "D3") == 0)
	{
		sPort = sBasePort;
		cBit = 8;
	}
	else if (stricmp(lzBuf, "D4") == 0)
	{
		sPort = sBasePort;
		cBit = 16;
	}
	else if (stricmp(lzBuf, "D5") == 0)
	{
		sPort = sBasePort;
		cBit = 32;
	}
	else if (stricmp(lzBuf, "D6") == 0)
	{
		sPort = sBasePort;
		cBit = 64;
	}
	else if (stricmp(lzBuf, "D7") == 0)
	{
		sPort = sBasePort;
		cBit = 128;
	}
	// Etat
	else if (stricmp(lzBuf, "/ERROR") == 0)
	{
		sPort = sBasePort+1;
		cBit = 8;
	}
	else if (stricmp(lzBuf, "SELECT") == 0)
	{
		sPort = sBasePort+1;
		cBit = 16;
	}
	else if (stricmp(lzBuf, "PE") == 0)
	{
		sPort = sBasePort+1;
		cBit = 32;
	}
	else if (stricmp(lzBuf, "/ACK") == 0)
	{
		sPort = sBasePort+1;
		cBit = 64;
	}
	else if (stricmp(lzBuf, "BUSY") == 0)
	{
		sPort = sBasePort+1;
		cBit = 128;
	}
	// Contr�le
	else if (stricmp(lzBuf, "/STROBE") == 0)
	{
		sPort = sBasePort+2;
		cBit = 1;
	}
	else if (stricmp(lzBuf, "/AUTOFEED") == 0)
	{
		sPort = sBasePort+2;
		cBit = 2;
	}
	else if (stricmp(lzBuf, "/INIT") == 0)
	{
		sPort = sBasePort+2;
		cBit = 4;
	}
	else if (stricmp(lzBuf, "/SELECT IN") == 0)
	{
		sPort = sBasePort+2;
		cBit = 8;
	}
	// Aucun
	else
	{
		sPort = 0;
		cBit = 0;
	}
}