#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

// Klavye kisayollari
enum
{
  KEY_ESC     = 27,
  ARROW_UP    = 256 + 72,
  ARROW_DOWN  = 256 + 80,
  ARROW_LEFT  = 256 + 75,
  ARROW_RIGHT = 256 + 77,
  KEY_ENTER   = 13,
  KEY_BACKSPACE = 8,
  KEY_SPACE = 32,
  CTRL_A = 1,
  CTRL_S = 19,
  CTRL_V = 22,
  CTRL_X = 24,
  CTRL_C = 3,
  KEY_TAB = 9
};

// Girilen harfleri birbirine baglamak icin olusturulan yapi
typedef struct Liste {
    int harf;
    int x;
    int y;
    struct Liste *sag;
    struct Liste *sol;
} Liste;

Liste *ilk = NULL, *son = NULL;

int satirSayisi = 0;
int harfSayisi = 0;

void ekranBaslik(char baslik[]) {
    SetConsoleTitle(baslik);
}

// Ekran yapilandirmalari
void ekranBoyutu() {
    HANDLE wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE rHnd = GetStdHandle(STD_INPUT_HANDLE);

    SMALL_RECT windowSize = {0, 800, 600, 0};
    SetConsoleWindowInfo(wHnd, 1, &windowSize);

    COORD bufferSize = {10, 10};
    SetConsoleScreenBufferSize(wHnd, bufferSize);
}

void cursor(int x, int y) {
   HANDLE wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = TRUE;
   SetConsoleCursorInfo(wHnd, &info);

   COORD cursor = {x, y};
   SetConsoleCursorPosition(wHnd, cursor);
}

int klavyeKontrol(int ch) {
  if ( ch == 0 || ch == 224 )
    ch = 256 + _getch();

  return ch;
}

void girisEkrani(void){
    int secim;
    printf("\n\n1. Yeni Belge\n2. Belge Ac\n3. Cikis\n\n>>> ");
    scanf("%d", &secim);
    switch(secim) {
        case 1:
            printf("Secim: 1\n");
            yeniBelge();
            break;
        case 2:
            printf("Secim: 2\n");
            belgeAc();
            break;
        case 3:
            printf("Secim: 3\n");
            exit(1);
            break;
        default:
            printf("Hatali giris! Lütfen dogru secim yapiniz!");
            Sleep(2000);
            system("CLS");
            girisEkrani();
            break;
    }
}

void yeniBelge(void) {
    system("CLS");

    OPENFILENAME ofn;
    char fileName[MAX_PATH] = "";
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "Text Files (*.txt*)\0*.txt*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "";

    GetSaveFileName(&ofn);
    ekranBaslik(fileName);

    klavyedenOku(fileName);

}



void belgeAc(void) {
    printf("Belge Ac\n");

    OPENFILENAME ofn;
    char fileName[MAX_PATH] = "";
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "Text Files (*.txt*)\0*.txt*\0";
    ofn.nFilterIndex = 0;
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
    ofn.lpstrDefExt = "";

    GetOpenFileName(&ofn);
    ekranBaslik(fileName);

    FILE *file;

    if((file = fopen(fileName, "w")) == NULL) {
        puts("Dosya acilamadi!\n");
        exit(1);
    }
}

void dosyayaKaydet(char dosyaAdres[]) {
    FILE *file;

    if((file = fopen(dosyaAdres, "w")) == NULL) {
        puts("Dosya acilamadi!\n");
        exit(1);
    }

    Liste *temp;
    temp = ilk;

    if(ilk != NULL) {
        while(temp) {
            fputc(temp->harf, file);
            temp = temp->sag;
        }
    }
    fclose(file);

}

void klavyedenOku(char dosyaAdres[]) {
    Liste *okunan;
    int harf;
    int x = 0, y = 0;

    while(1) {
        okunan = (Liste*) malloc(sizeof(Liste));
        harf = _getch();
        if(( harf = klavyeKontrol(harf) ) != KEY_ESC) {
            switch(harf) {
                case ARROW_UP:
                    y--;
                  cursor(x, y);
                  break;
                case ARROW_DOWN:
                    y++;
                  cursor(x, y);
                  break;
                case ARROW_LEFT:
                    x--;
                  cursor(x, y);
                  break;
                case ARROW_RIGHT:
                    x++;
                  cursor(x, y);
                  break;
                case KEY_ENTER:
                    x=0;
                    y++;
                  cursor(x, y);
                    break;
                case KEY_TAB:
                    printf("TAB");
                    break;
                case KEY_BACKSPACE:
                    printf("BACKSPACE");
                    break;
                case KEY_SPACE:
                    printf("SPACE");
                    break;
                case CTRL_A:
                    printf("CTRL+A");
                    break;
                case CTRL_C:
                    printf("CTRL+C");
                    break;
                case CTRL_S:                    // Dosyaya kaydet
                    dosyayaKaydet(dosyaAdres);
                    break;
                case CTRL_X:
                    printf("CTRL+X");
                    break;
                case CTRL_V:
                    printf("CTRL+V");
                    break;
                case KEY_ESC:
                    printf("ESC");
                    exit(1);
                    break;
                default:
                    x++;
                    okunan->harf = harf;
                    _putch(harf);
                    listeyeEkle(okunan, x, y);
                    break;
            if(y<0) y=0;
            if(x<0) x=0;

            }
        } else {
            break;
        }

    }

}
void listeyeEkle(Liste *eklenecek, int x, int y) {
    eklenecek->x = x;
    eklenecek->y = y;

    if(ilk != NULL) {
        son->sag = eklenecek;
        eklenecek->sol = son;
        son = eklenecek;
        son->sag = NULL;
    } else {
        ilk = eklenecek;
        son = eklenecek;
        ilk->sag = NULL;
        ilk->sol = NULL;
    }
}

void listeyiYazdir(void) {
    Liste *temp;
    temp = ilk;

    if(ilk == NULL) {
        printf("Liste bos!\n");
        exit(1);
    }

    while(temp) {
        _putch(temp->harf);
        temp = temp->sag;
    }
}

int main(void)
{
    int x = 0, y = 0;           // Cursor (0,0) noktasýndan baþlýyor.

    ekranBaslik("Text Editor @BerkayEzdemir");
    cursor(x, y);
    girisEkrani();

    listeyiYazdir();

    printf("\nSon\n");

    return 0;
}
