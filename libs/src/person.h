struct person
{
    char code[50], gender;
    person *next;
    medicineDescriptor medicines;
};

struct personDescriptor
{
    person *start, *current;
    int quantity;
};
