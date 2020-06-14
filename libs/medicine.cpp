#include "medicine.h"


medicineDescriptor createMedicineDescriptor(){
    medicineDescriptor medicines;
    medicines.start = medicines.current = NULL;
    medicines.quantity = 0;

    return medicines;
}

medicine *createMedicine(char name[], char last_buy[]){
    medicine *_medicine = new medicine;
    strcpy(_medicine->name, name);
    strcpy(_medicine->last_buy, last_buy);
    _medicine->next = NULL;

    return _medicine;
}

medicineDescriptor getPersonMedicines(person *_person){
    medicineDescriptor meds = _person->medicines;
    moveToFirstMedicine(meds);
    return meds;
}

void insertMedicine(person *_person, char name[], char last_buy[]){
    medicineDescriptor medicines = getPersonMedicines(_person);
    moveToFirstMedicine(medicines);
    while(getCurrentMedicine(medicines) != NULL && stricmp(getCurrentMedicine(medicines)->name, name) != 0)
        moveToNextMedicine(medicines);
    
    if(stricmp(getCurrentMedicine(medicines)->name, name) != 0){
        medicine *_medicine = createMedicine(name, last_buy);

        medicine *aux = getFirstMedicine(medicines);
        medicines.start = _medicine;
        _medicine->next = aux;
            

        medicines.quantity++;
        medicines.current = _medicine;
    }

    _person->medicines = medicines;

}

medicine *getFirstMedicine(medicineDescriptor medicines){
    return medicines.start;
}
medicine *getCurrentMedicine(medicineDescriptor medicines){
    return medicines.current;
}
medicine *getNextMedicine(medicineDescriptor medicines){
    return getCurrentMedicine(medicines)->next;
}

void moveToNextMedicine(medicineDescriptor &medicines){
    //if(!isMedicineTheLast(medicines))
        medicines.current = medicines.current->next;
}

void moveToFirstMedicine(medicineDescriptor &medicines){
    medicines.current = medicines.start;
}

int isMedicineTheLast(medicineDescriptor medicines){
    return medicines.current->next == NULL;
}
int isMedicineTheFirst(medicineDescriptor medicines){
    return medicines.current == medicines.start;
}

int isEndOfMedicines(medicineDescriptor medicines){
    return medicines.current == NULL;
}

medicine *getMedicineByNode(medicineDescriptor medicines, int nodes){
    medicine *_medicine = getFirstMedicine(medicines);
    while(nodes-- > 0) _medicine = _medicine->next;

    return _medicine;
}