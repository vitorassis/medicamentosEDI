#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interio.h"
#include "libs/libs.h"
#include "data/loadData.cpp"

#define version "0.1.5"

void showArvoreInterface(countryDescriptor paises, breadcrumb show){
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

                        if(peopleTecla < peopleInpage+1){
                            if((pagePeople > 0 && peopleTecla < peopleInpage-1) ||
                            (pagePeople == 0 && peopleTecla < peopleInpage)){
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

void showSearchCountryInterface(countryDescriptor paises, breadcrumb rel){
    menu _menu = setMenu(9), submenu = setMenu(15);

    char pesquisa[30];
    int opc, subopc;
    do{
        opc = 0;
        clearCanvas();
        showBreadcrumbs(setBreadcrumb("Buscar Pais", &rel));
        gotoxy(10, 6);printf("Pais: ");
        readString(pesquisa, 16, 6, 30);
        if(strcmp(pesquisa, "\0")){
            do{
                searchCountries(paises, pesquisa);
                if(paises.sectionQuantity == 0)
                    showToast("PAIS NAO ENCONTRADO!", TOAST_ERROR);
                else{
                    moveToFirstCountrySection(paises);
                    clearMenuOptions(_menu);
                    removeToast();
                    while(!isEndOfCountriesSection(paises)){
                        addMenuOption(_menu, getCurrentCountry(paises)->name);

                        moveToNextCountry(paises);
                    }
                    addMenuOption(_menu, "", 0);
                    addMenuOption(_menu, "VOLTAR");
                    opc = showMenu(_menu, opc);

                    if(opc != paises.sectionQuantity+1){
                        country *aux;
                        medicineDescriptor remedios, remAux;

                        showToast("GERANDO RELATORIO...", TOAST_WARNING);   

                        FILE *arq = fopen("rel2.txt", "w");
                        personDescriptor pessoas;
                        paises.current = getCountryByNodeInSection(paises, opc);
                        aux = createCountry("aux");
                        insertPerson(aux, "000-00-0000", 'm');

                        fprintf(arq, "%s\n", getCurrentCountry(paises));
                        
                        pessoas = getCountryPeople(getCurrentCountry(paises));
                        while(!isEndOfPeople(pessoas)){
                            remedios = getPersonMedicines(getCurrentPerson(pessoas));
                            while(!isEndOfMedicines(remedios)){
                                insertMedicine(getCurrentPerson(getCountryPeople(aux)), getCurrentMedicine(remedios)->name, getCurrentMedicine(remedios)->last_buy);

                                moveToNextMedicine(remedios);
                            }

                            moveToNextPerson(pessoas);
                        } 
                        remAux = getPersonMedicines(getCurrentPerson(getCountryPeople(aux)));
                        while(!isEndOfMedicines(remAux)){
                            fprintf(arq, "\t%s\n", getCurrentMedicine(remAux)->name);

                            moveToNextMedicine(remAux);
                        }

                        fprintf(arq, "\n\n");
                        

                        fclose(arq);
                        showToast("RELATORIO FINALIZADO!", TOAST_SUCCESS);

                    }
                }

            }while(paises.sectionQuantity != 0 && opc != paises.sectionQuantity+1);
        }
    }while(strcmp(pesquisa, "\0"));
    removeToast();
}

void showRelatorioInterface(countryDescriptor paises, breadcrumb show){
    FILE *arq;
    personDescriptor pessoas;
    breadcrumb rel = setBreadcrumb("Relatorio", &show);
    showBreadcrumbs(rel);
    menu exibeMenu = setMenu(10);
    int tecla=0;
    addMenuOption(exibeMenu, "Usuarios por sexo por pais (rel1)");
    addMenuOption(exibeMenu, "Medicamento por pais (rel2)");
    addMenuOption(exibeMenu, "", 0);
    addMenuOption(exibeMenu, "VOLTAR");
    do{
        tecla = showMenu(exibeMenu, tecla);

        clearCanvas();
        switch(tecla){
            case 0:
                arq = fopen("rel1.txt", "w");
                
                showToast("GERANDO RELATORIO...", TOAST_WARNING);   

                moveToFirstCountry(paises);
                while(!isEndOfCountries(paises)){

                    fprintf(arq, "%s\n", getCurrentCountry(paises));
                    fprintf(arq, "\tFeminino:\n");
                    
                    pessoas = getCountryPeople(getCurrentCountry(paises));
                    moveToFirstPerson(pessoas);
                    while(!isEndOfPeople(pessoas)){
                        if(toupper(getCurrentPerson(pessoas)->gender) == 'F')
                            fprintf(arq, "\t\t%s\n", getCurrentPerson(pessoas)->code);
                        moveToNextPerson(pessoas);
                    } 

                    fprintf(arq, "\tMasculino:\n");
                    moveToFirstPerson(pessoas);
                    while(!isEndOfPeople(pessoas)){
                        if(toupper(getCurrentPerson(pessoas)->gender) == 'M')
                            fprintf(arq, "\t\t%s\n", getCurrentPerson(pessoas)->code);
                        moveToNextPerson(pessoas);
                    } 

                    fprintf(arq, "\n\n");
                    moveToNextCountry(paises);
                }

                fclose(arq);
                showToast("RELATORIO FINALIZADO!", TOAST_SUCCESS);
                break;
            case 1:
                showSearchCountryInterface(paises, rel);
                break;
        }
    }while(tecla != 3);
}

void showShowInterface(countryDescriptor paises, breadcrumb home){
    breadcrumb show = setBreadcrumb("Exibir", &home);
    showBreadcrumbs(show);
    menu exibeMenu = setMenu(10);
    int tecla=0;
    addMenuOption(exibeMenu, "Gerar Relatorio");
    addMenuOption(exibeMenu, "Visualizar arvore");
    addMenuOption(exibeMenu, "", 0);
    addMenuOption(exibeMenu, "VOLTAR");
    do{
        tecla = showMenu(exibeMenu, tecla);

        clearCanvas();
        switch(tecla){
            case 0:
                showRelatorioInterface(paises, show);
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

void showDeleteMedicineInterface(countryDescriptor &paises, person *usuario, breadcrumb del){
    menu _menu = setMenu(9), submenu = setMenu(15);
    addMenuOption(submenu, "Remover medicamento");//0
    addMenuOption(submenu, "", 0);
    addMenuOption(submenu, "VOLTAR");      //2
    int opc=0, subopc;
    medicineDescriptor medicines = getPersonMedicines(usuario);
    do{
        moveToFirstMedicine(medicines); 
        clearMenuOptions(_menu);
        clearCanvas();
        showBreadcrumbs(del);
        printCenter("Medicamento:", 7);
            gotoxy(30, 10);printf("QTD: %d", medicines.quantity);
        while(!isEndOfMedicines(medicines)){
            addMenuOption(_menu, getCurrentMedicine(medicines)->name);
            moveToNextMedicine(medicines);
        }
        addMenuOption(_menu, "", 0);
        addMenuOption(_menu, "VOLTAR");
        opc = showMenu(_menu, opc);
        if(opc != medicines.quantity+1){
            subopc = 0;

                int popx, popy;
                drawPopUpWindow(30, 10, popx, popy);

                printCenter("Acoes:", popy+1);
                subopc = showMenu(submenu, subopc);
                switch(subopc){
                    case 0:
                        removeMedicine(medicines, getMedicineByNode(medicines, opc));
                        usuario->medicines = medicines;
                        wipeTrashCountryData(paises);
                        break;
                }

                removePopUpWindow(30, 10);
        }
    }while(medicines.quantity > 0 && opc != medicines.quantity+1);
}

void showDeletePersonInterface(countryDescriptor &paises, country* pais, breadcrumb del){
    menu _menu = setMenu(9), submenu = setMenu(15);
    addMenuOption(submenu, "Ver medicamentos"); //0
    addMenuOption(submenu, "Remover pessoa");//1
    addMenuOption(submenu, "", 0);
    addMenuOption(submenu, "VOLTAR");      //3
    int opc, subopc, pagenum = 0, peoplePerPage = 15, peopleinPage=0;
    personDescriptor people;
    do{
        people = getCountryPeople(pais);
        people.current = getPersonByNode(people, peoplePerPage*pagenum);
        peopleinPage=0;
        clearMenuOptions(_menu);
        opc = 0;
        clearCanvas();
        showBreadcrumbs(del);
        printCenter("Pessoa:", 7);
        while(!isEndOfPeople(people) && peopleinPage++ < peoplePerPage){
            addMenuOption(_menu, getCurrentPerson(people)->code);
            moveToNextPerson(people);
        }
        if(pagenum > 0)
            addMenuOption(_menu, "ANTERIOR");
        else
            addMenuOption(_menu, "", 0);

        if(!isEndOfPeople(people))
            addMenuOption(_menu, "PROXIMA");
        else
            addMenuOption(_menu, "", 0);

        addMenuOption(_menu, "VOLTAR");

        opc = showMenu(_menu, opc);
        if(opc != peopleinPage+2){
            if(opc ==  peopleinPage)
                pagenum --;
            else if(opc ==  peopleinPage+1)
                pagenum++;

            else{
                subopc = 0;

                int popx, popy;
                drawPopUpWindow(30, 10, popx, popy);

                printCenter("Acoes:", popy+1);
                subopc = showMenu(submenu, subopc);
                switch(subopc){
                    case 0:
                        showDeleteMedicineInterface(paises, getPersonByNode(people, (peoplePerPage*pagenum)+opc), setBreadcrumb(getPersonByNode(people, (peoplePerPage*pagenum)+opc)->code, &del));
                        break;
                    case 1:
                        removePerson(people, getPersonByNode(people, (peoplePerPage*pagenum)+opc));
                        pais->people = people;
                        wipeTrashCountryData(paises);
                        break;
                }

                removePopUpWindow(30, 10);
            }
        }

    }while(getCountryPeople(pais).quantity > 0 && opc != peopleinPage+2);
}

void showDeleteCountryInterface(countryDescriptor &paises, breadcrumb home){
    breadcrumb del = setBreadcrumb("Excluir", &home);
    
    menu _menu = setMenu(9), submenu = setMenu(15);
    addMenuOption(submenu, "Ver pessoas"); //0
    addMenuOption(submenu, "Remover pais");//1
    addMenuOption(submenu, "", 0);
    addMenuOption(submenu, "VOLTAR");      //3

    char pesquisa[30];
    int opc, subopc;
    do{
        opc = 0;
        clearCanvas();
        showBreadcrumbs(del);
        gotoxy(10, 6);printf("Pais: ");
        readString(pesquisa, 16, 6, 30);
        if(strcmp(pesquisa, "\0")){
            do{
                searchCountries(paises, pesquisa);
                if(paises.sectionQuantity == 0)
                    showToast("PAIS NAO ENCONTRADO!", TOAST_ERROR);
                else{
                    moveToFirstCountrySection(paises);
                    clearMenuOptions(_menu);
                    removeToast();
                    while(!isEndOfCountriesSection(paises)){
                        addMenuOption(_menu, getCurrentCountry(paises)->name);

                        moveToNextCountry(paises);
                    }
                    addMenuOption(_menu, "", 0);
                    addMenuOption(_menu, "VOLTAR");
                    opc = showMenu(_menu, opc);

                    if(opc != paises.sectionQuantity+1){
                        subopc = 0;

                        int popx, popy;
                        drawPopUpWindow(30, 10, popx, popy);

                        printCenter("Acoes:", popy+1);
                        subopc = showMenu(submenu, subopc);
                        switch(subopc){
                            case 0:
                                showDeletePersonInterface(paises, getCountryByNodeInSection(paises, opc), setBreadcrumb(getCountryByNodeInSection(paises, opc)->name, &del));
                                break;
                            case 1:
                                removeCountry(paises, getCountryByNodeInSection(paises, opc));
                        }

                        removePopUpWindow(30, 10);
                    }
                }

            }while(paises.sectionQuantity != 0 && opc != paises.sectionQuantity+1);
        }
    }while(strcmp(pesquisa, "\0"));
    removeToast();
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
                showDeleteCountryInterface(paises, home);
                break;
        }
        
    }while(opcao != 4);

    return 0;
}
