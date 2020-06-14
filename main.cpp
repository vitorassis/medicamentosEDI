#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interio.h"
#include "libs/libs.h"
#include "data/loadData.cpp"

#define version "0.1.0"

void showArvoreInterface(countryDescriptor paises, breadcrumb show){
    showBreadcrumbs(setBreadcrumb("Arvore", &show));

    menu countryMenu = setMenu(5, 5);
    menu peopleMenu = setMenu(5, 45);
    menu medicineMenu = setMenu(5, 63);
    char text[60];
    int countryTecla, peopleTecla, medicineTecla;
    personDescriptor pessoas;
    medicineDescriptor remedios;

    drawLine(4,canvasSetting.height-5, 75, 1);

    moveToFirstCountry(paises);
    while(!isEndOfCountries(paises)){
        sprintf(text, "%s (%d)", getCurrentCountry(paises)->name, getCurrentCountry(paises)->people.quantity);
        addMenuOption(countryMenu, text);
        moveToNextCountry(paises);
    }
    addMenuOption(countryMenu, "VOLTAR");

    do{
        peopleTecla = 0;
        countryTecla = showMenu(countryMenu, countryTecla);

        if(countryTecla != paises.quantity){
            country *selectedCountry = getCountryByNode(paises, countryTecla);

            clearMenuOptions(peopleMenu);
            pessoas = getCountryPeople(selectedCountry);
            
            while(!isEndOfPeople(pessoas)){
                sprintf(text, "%s (%c)", getCurrentPerson(pessoas)->code, getCurrentPerson(pessoas)->gender);
                addMenuOption(peopleMenu, text);
                moveToNextPerson(pessoas);
            }
            addMenuOption(peopleMenu, "VOLTAR");

            do{
                medicineTecla = 0;
                peopleTecla = showMenu(peopleMenu, peopleTecla);

                if(peopleTecla != pessoas.quantity){
                    person *selectedPerson = getPersonByNode(pessoas, peopleTecla);

                    clearMenuOptions(medicineMenu);
                    remedios = getPersonMedicines(selectedPerson);

                    while(!isEndOfMedicines(remedios)){
                        addMenuOption(medicineMenu, getCurrentMedicine(remedios)->name);
                        moveToNextMedicine(remedios);
                    }
                    addMenuOption(medicineMenu, "VOLTAR");

                    do{
                        medicineTecla = showMenu(medicineMenu, medicineTecla);
                        
                        clearCoordinates(76, 5, canvasSetting.height-5);

                        if(medicineTecla != remedios.quantity){
                            medicine *selectedMedicine = getMedicineByNode(remedios, medicineTecla);

                            gotoxy(80, 10);printf("USUARIO:");
                            gotoxy(77, 11);printf("Pais: %s", selectedCountry);
                            gotoxy(77, 12);printf("Codigo: %s", selectedPerson->code);
                            gotoxy(77, 13);printf("Sexo: %c", selectedPerson->gender);
                            gotoxy(80, 15);printf("REMEDIO:");
                            gotoxy(77, 16);printf("Nome: %s", selectedMedicine->name);
                            gotoxy(77, 16);printf("Ultima Compra: %s", selectedMedicine->last_buy);
                        }else
                            clearCoordinates(64, 5, 74, canvasSetting.height-5);
                    }while(medicineTecla != remedios.quantity);
                }else
                    clearCoordinates(45, 5, 63, canvasSetting.height-5);

            }while(peopleTecla != pessoas.quantity);

        }else{
            clearCanvas();
        }
        
    }while(countryTecla != paises.quantity);  
}

void showShowInterface(countryDescriptor paises, breadcrumb home){
    breadcrumb show = setBreadcrumb("Exibir", &home);
    showBreadcrumbs(show);
    menu exibeMenu = setMenu(10);
    int tecla=0;
    addMenuOption(exibeMenu, "Gerar Relatorio", 0);
    addMenuOption(exibeMenu, "Visulizar arvore");
    addMenuOption(exibeMenu, "", 0);
    addMenuOption(exibeMenu, "VOLTAR");
    do{
        tecla = showMenu(exibeMenu, tecla);

        clearCanvas();
        switch(tecla){
            case 0:
                break;
            case 1:
                showArvoreInterface(paises, show);
                break;
        }
    }while(tecla != 3);
}

void showInsertInterface(countryDescriptor &paises, breadcrumb home){
    showBreadcrumbs(setBreadcrumb("Inserir Usuario", &home));

    char pais[50], remedio[50], codigo[50], sexo, compra[11];

    gotoxy(10,8);printf("Pais: ");
    gotoxy(50,8);printf("Remedio: ");
    gotoxy(10,12);printf("Codigo: ");
    gotoxy(50,12);printf("Sexo: ");
    gotoxy(30,16);printf("Ultima Compra: ");

    do{
        readString(pais, 16, 8, 30);
        if(strlen(pais) == 0)
            showToast("PAIS ESTA NULO", TOAST_ERROR);
    }while(strlen(pais) == 0);
    removeToast();

    do{
        readString(remedio, 59, 8, 30);
        if(strlen(remedio) == 0)
            showToast("REMEDIO ESTA NULO", TOAST_ERROR);
    }while(strlen(remedio) == 0);
    removeToast();

    do{
        readMaskedString(codigo, "ddd-dd-dddd", 18, 12);
        if(codigo[10] == 0)
            showToast("CODIGO ESTA NULO", TOAST_ERROR);
    }while(codigo[10] == 0);
    removeToast();

    do{
        sexo = readChar(56, 12);
        if(toupper(sexo) != 'F' && toupper(sexo) != 'M')
            showToast("TENTE M OU F", TOAST_ERROR);
    }while(toupper(sexo) != 'F' && toupper(sexo) != 'M');
    removeToast();

    char code[12];
    strcpy(code, codigo);

    do{
        readMaskedString(compra, "dd/dd/dddd", 45, 16);
        if(compra[9] == 0)
            showToast("DATA ESTA NULA", TOAST_ERROR);
    }while(compra[9] == 0);
    removeToast();

    gotoxy(10, 19);printf("Confirma esses dados? (S/n) ");
    char conf;
    do{
        conf = readChar(39, 19);
        if(toupper(conf) != 'S' && toupper(conf) != 'N')
            showToast("USE S OU N", TOAST_ERROR);
    }while(toupper(conf) != 'S' && toupper(conf) != 'N');
    removeToast();

    if(toupper(conf) == 'S'){

        insertCountry(paises, pais);
        insertPerson(getCurrentCountry(paises), code, sexo);
        insertMedicine(getCurrentPerson(getCountryPeople(getCurrentCountry(paises))), remedio, compra);
    }
}

int main(){
    countryDescriptor paises = createCountryDescriptor();

    insertDataFromCSV(paises);

    setCanvas('#', 1, 1);
    char title[40];
    sprintf(title, "Usuarios de Medicamentos v%s", version);
    showTitle(title);
    breadcrumb home = setBreadcrumb("Inicio");
    drawCanvas();
    menu mainMenu = setMenu(10);
    int opcao = 0;
    do{
        clearCanvas();
        showBreadcrumbs(home);
        clearMenuOptions(mainMenu);
        addMenuOption(mainMenu, "Inserir Usuario");
        addMenuOption(mainMenu, "Exibir", !isCountriesEmpty(paises));
        addMenuOption(mainMenu, "Excluir", !isCountriesEmpty(paises));
        addMenuOption(mainMenu, "", 0);
        addMenuOption(mainMenu, "SAIR");

        opcao = showMenu(mainMenu, opcao);

        clearCanvas();
        switch(opcao){
            case 0:
                showInsertInterface(paises, home);
                break;
            case 1:
                showShowInterface(paises, home);
                break;
            // case 2:
            //     showDeleteInterface(paises, home);
            //     break;
        }
        
    }while(opcao != 4);

    return 0;
}
