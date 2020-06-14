
struct medicine
{
    char name[100], last_buy[11];
    medicine *next;
};

struct medicineDescriptor
{
    medicine *start, *current;
    int quantity;
};