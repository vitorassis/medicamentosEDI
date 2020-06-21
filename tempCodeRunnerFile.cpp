    searchCountries(paises, pesquisa);
            if(paises.sectionQuantity == 0)
                showToast("PAIS NAO ENCONTRADO!", TOAST_ERROR);
            else{

                do{
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
                        
                    }

                }while(opc != paises.sectionQuantity+1);
        