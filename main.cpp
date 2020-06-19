#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interio.h"
#include "libs/libs.h"
#include "data/loadData.cpp"

#define version "0.1.5"

void showArvoreInterface(countryDescriptor paises, breadcrumb show , int _delete = 0, char delete_node = ' '){
    showBreadcrumbs(setBreadcrumb("Arvore", &show));

    menu letterMenu = setMenu(5, 5);
    menu countryMenu = setMenu(5, 10);
    menu peopleMenu = setMenu(5, 30);
    menu medicineMenu = setMenu(5, 50);
    char text[60], letters[27], conf;
    int letterssize = 0;
    int letterTecla, countryTecla, peopleTecla, medicineTecla, pagePeople, peoplePerPage = 15, peopleInpage;
    personDescriptor pessoas;
    medicineDescriptor remedios;

    drawLine(4,canvasSetting.height-5, 75, 1);


    do{
        clearMenuOptions(letterMenu);
        getCountriesFirstLetters(paises, letters, letterssize);
        for(int i=0; letters[i]; i++){
            sprintf(text, "%c", letters[i]);
            addMenuOption(letterMenu, text);
        }
        addMenuOption(letterMenu, "VOLTAR");

        letterTecla = showMenu(letterMenu, letterTecla);

        if(letterTecla != letterssize){
            countryTecla = 0;
            

            do{
                pagePeople = 0;

                clearMenuOptions(countryMenu);
                getCountriesFromFirstLetter(paises, letters[letterTecla]);
                while(!isEndOfCountriesSection(paises)){
                    sprintf(text, "%s (%d)", getCurrentCountry(paises)->name, getCurrentCountry(paises)->people.quantity);
                    addMenuOption(countryMenu, text);
                    moveToNextCountry(paises);
                }
                addMenuOption(countryMenu, "VOLTAR");

                countryTecla = showMenu(countryMenu, countryTecla);

                if(_delete && delete_node == 'C'){
                    if(countryTecla != paises.sectionQuantity){
                        do{
                            gotoxy(30, 19);printf("Deseja remover esse pais? (S/N)");
                            conf = readChar(60, 19);
                        }while(toupper(conf) != 'N' && toupper(conf) != 'S');
                        if(toupper(conf) == 'S')
                            removeCountry(paises, getCountryByNodeInSection(paises, countryTecla));
                        clearCoordinates(10, 5, 25, canvasSetting.height-5);
                    }
                }else
                    if(countryTecla != paises.sectionQuantity){
                        country *selectedCountry = getCountryByNodeInSection(paises, countryTecla);

                        pessoas = getCountryPeople(selectedCountry);
                        
                        do{
                            peopleInpage = peopleTecla = 0;
                            clearMenuOptions(peopleMenu);
                            pessoas.current = getPersonByNode(pessoas, pagePeople*peoplePerPage);

                            while(!isEndOfPeople(pessoas) && peopleInpage++ < peoplePerPage){
                                sprintf(text, "%s (%d)", getCurrentPerson(pessoas)->code, getCurrentPerson(pessoas)->medicines.quantity);
                                addMenuOption(peopleMenu, text);
                                moveToNextPerson(pessoas);
                            }
                            if(pagePeople > 0)
                                addMenuOption(peopleMenu, "ANTERIOR");
                            else
                                addMenuOption(peopleMenu, "", 0);
                                
                            if(!isEndOfPeople(pessoas))
                                addMenuOption(peopleMenu, "PROXIMA");
                            else
                                addMenuOption(peopleMenu, "", 0);

                            addMenuOption(peopleMenu, "VOLTAR");

                            medicineTecla = 0;
                            peopleTecla = showMenu(peopleMenu, peopleTecla);

                            if(_delete && delete_node == 'P'){
                                if(peopleTecla < peopleInpage+1){
                                    do{
                                        gotoxy(30, 19);printf("Deseja remover essa pessoa? (S/N)");
                                        conf = readChar(69, 19);
                                    }while(toupper(conf) != 'N' && toupper(conf) != 'S');
                                    if(toupper(conf) == 'S')
                                        removePerson(selectedCountry->people, getPersonByNode(pessoas, (pagePeople*peoplePerPage) + peopleTecla));
                                    clearCoordinates(25, 5, 63, canvasSetting.height-5);

                                }
                            }else
                                if(peopleTecla < peopleInpage+1){
                                    if(peopleTecla != peopleInpage-1 && peopleTecla != peopleInpage){
                                        person *selectedPerson = getPersonByNode(pessoas, (pagePeople*peoplePerPage) + peopleTecla);


                                        do{
                                            clearMenuOptions(medicineMenu);
                                            remedios = getPersonMedicines(selectedPerson);

                                            while(!isEndOfMedicines(remedios)){
                                                getTruncatedName(getCurrentMedicine(remedios), text, 20);
                                                addMenuOption(medicineMenu, text);
                                                moveToNextMedicine(remedios);
                                            }
                                            addMenuOption(medicineMenu, "VOLTAR");

                                            medicineTecla = showMenu(medicineMenu, medicineTecla);
                                            if(_delete && delete_node == 'M'){
                                                if(medicineTecla != remedios.quantity){
                                                    do{
                                                        gotoxy(30, 19);printf("Deseja remover esse medicamento? (S/N)");
                                                        conf = readChar(70, 19);
                                                    }while(toupper(conf) != 'N' && toupper(conf) != 'S');
                                                    if(toupper(conf) == 'S')
                                                        removeMedicine(selectedPerson->medicines, getMedicineByNode(remedios, medicineTecla));
                                                }
                                            }else{
                                                clearCoordinates(76, 5, canvasSetting.width-3, canvasSetting.height-5);

                                                if(medicineTecla != remedios.quantity){
                                                    medicine *selectedMedicine = getMedicineByNode(remedios, medicineTecla);

                                                    gotoxy(80, 10);printf("USUARIO:");
                                                    gotoxy(77, 11);printf("Pais: %s", selectedCountry);
                                                    gotoxy(77, 12);printf("Codigo: %s", selectedPerson->code);
                                                    gotoxy(77, 13);printf("Sexo: %c", selectedPerson->gender);
                                                    gotoxy(80, 15);printf("REMEDIO:");
                                                    gotoxy(77, 16);printf("Nome: %s", selectedMedicine->name);
                                                    gotoxy(77, 17);printf("Ultima Compra: %s", selectedMedicine->last_buy);
                                                }else
                                                    clearCoordinates(45, 5, 74, canvasSetting.height-5);
                                            }
                                        }while(medicineTecla != remedios.quantity);
                                    }else{
                                        if(peopleTecla == peopleInpage-1){
                                            pagePeople --;
                                        }else{
                                            pagePeople ++;
                                        }clearCoordinates(25, 5, 63, canvasSetting.height-5);
                                    }
                                    
                                }else
                                    clearCoordinates(25, 5, 63, canvasSetting.height-5);

                        }while(peopleTecla < peopleInpage+1);

                    }else{
                        clearCoordinates(10, 5, 25, canvasSetting.height-5);
                    }
                
            }while(countryTecla != paises.sectionQuantity);  
        }else{
            clearCoordinates(5, 5, 25, canvasSetting.height-5);
        }
    }while(letterTecla != letterssize);
    clearCanvas();
}

void showShowInterface(countryDescriptor paises, breadcrumb home){
    breadcrumb show = setBreadcrumb("Exibir", &home);
    showBreadcrumbs(show);
    menu exibeMenu = setMenu(10);
    int tecla=0;
    addMenuOption(exibeMenu, "Gerar Relatorio", 0);
    addMenuOption(exibeMenu, "Visualizar arvore");
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

void showDeleteInterface(countryDescriptor paises, breadcrumb home){
    breadcrumb del = setBreadcrumb("Excluir", &home);
    showBreadcrumbs(del);
    menu exibeMenu = setMenu(10);
    int tecla=0;
    addMenuOption(exibeMenu, "Remover pais");
    addMenuOption(exibeMenu, "Remover pessoa", getFirstCountry(paises)->people.quantity);
    addMenuOption(exibeMenu, "Remover medicamento", getPersonMedicines(getFirstPerson(getCountryPeople(getFirstCountry(paises)))).quantity);
    addMenuOption(exibeMenu, "", 0);
    addMenuOption(exibeMenu, "VOLTAR");
    do{
        tecla = showMenu(exibeMenu, tecla);

        clearCanvas();
        switch(tecla){
            case 0:
                showArvoreInterface(paises, setBreadcrumb("Pais", &del), 1, 'C');
                break;
            case 1:
                showArvoreInterface(paises, setBreadcrumb("Pessoa", &del), 1, 'P');
                break;
            case 2:
                showArvoreInterface(paises, setBreadcrumb("Medicamento", &del), 1, 'M');
        }
    }while(tecla != 4 && !isEndOfCountries(paises));
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
            case 2:
                showDeleteInterface(paises, home);
                break;
        }
        
    }while(opcao != 4);

    return 0;
}
