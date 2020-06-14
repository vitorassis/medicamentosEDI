void lineBreaker(char line[], char stock[], int &i){
    int strpos =0;
    while(line [i] != ','){
        stock[strpos++] =line[i++];
        stock[strpos] = 0;
    }
    i++;
}

void lineMedicineBreaker(char line[], char stock[][100], int &i){
    if(line[i] == '"'){
        int startI = i;
        int strpos = 0;
        int index = 0;
        while(!(line[i] == '"' && i > startI)){
            if(line[i]!= '"'){
                if(line[i] != ','){
                    stock[index][strpos++] = line[i]; 
                    stock[index][strpos] = 0;
                }else if(line[i] == ' '){
                    index++;
                    strpos = 0;
                }
            }
            i++;
        }
        stock[index+1][0] = 0;
        i++;
    }else
        lineBreaker(line, stock[0], i);
        stock[1][0] = 0;
}

void insertDataFromCSV(countryDescriptor &countries){
    FILE *file = fopen("data/DadosMedicamentos.csv", "r");

    char temp[200];
    char code[12], gender[2], meds[20][100], lastbuy[11], countryname[50]; 
    int i, j;
    if(file != NULL)
        while(!feof(file)){
            i=0;
            fgets(temp, 1024, file);

            lineBreaker(temp, code, i);
            lineBreaker(temp, gender, i);
            lineMedicineBreaker(temp, meds, i);
            lineBreaker(temp, lastbuy, i);
            lineBreaker(temp, countryname, i);
            

            printf("CODE: %s\n", code);
            printf("GENDER: %c\n", gender[0]);
            j =0;
            while(meds[j][0] != 0)
                printf("REMEDIO: %s\n", meds[j++]);
            printf("LASTBUY: %s\n", lastbuy);
            printf("COUNTRY: %s\n", countryname);

            insertCountry(countries, countryname);
            insertPerson(getCurrentCountry(countries), code, gender[0]);
            j =0;
            while(meds[j][0] != 0)
                insertMedicine(getCurrentPerson(getCountryPeople(getCurrentCountry(countries))),
                meds[j++], lastbuy);

        }

    fclose(file);
}