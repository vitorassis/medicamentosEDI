void lineBreaker(char line[], char stock[], int &i){
    int strpos =0;
    while(line [i] != ',' && line[i] != '\n'){
        stock[strpos++] =line[i++];
    }
    i++;
    stock[strpos] = 0;
}

void lineMedicineBreaker(char line[], char stock[][100], int &i){
    int count = 1;
    if(line[i] == '"'){
        char temp[200];
        int ti = 0;
        i++;
        int k = 0;

        while(line[i] != '"'){
            if(line[i-1] == ',' && line[i] == ' ')
                count++;
            else
                temp[ti++] = line[i];
            i++;
        }
        i+=2;
        strcat(temp, ",\0");

        for(int j = 0; j<count; j++){
            lineBreaker(temp, stock[j], k);
        }
    }else
        lineBreaker(line, stock[0], i);
    
    strcpy(stock[count], "\0");
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

            insertCountry(countries, countryname);
            insertPerson(getCurrentCountry(countries), code, gender[0]);
            j =0;
            while(meds[j][0] != 0)
                insertMedicine(getCurrentPerson(getCountryPeople(getCurrentCountry(countries))),
                meds[j++], lastbuy);

       }

    fclose(file);

    moveToFirstCountry(countries);

}