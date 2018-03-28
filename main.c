#include <stdio.h>
#include <memory.h>
#include "stdlib.h"
#include "log.h"
#include "pthread.h"
#include <time.h>
#include <unistd.h>



typedef struct Account {
    int a_id;
    int c_id;
    float amount;
    int day_limit;
    int operation_count;
    pthread_mutex_t lock;
    struct Account *next;

} Account;

typedef struct Customer{
    int id;
    int action;
    int a_id;
} Customer;

typedef struct Teller{
    int id;
    int action;
    pthread_mutex_t lock;
    struct Teller *next;
} Teller;
struct Account* accounts = NULL;

int nof_customers = 0;
int nof_tellers = 0;
int nof_simulation_days = 0;


int *customer_ids;
int *account_ids;

FILE * log;

int account_size = 0;

long clock_timestamp =0 ;
long past_timestamp = 0;
int stop_clock = 0 ;
int simulation_day= -1;
void push(struct Account** head_ref, int a_id, int c_id, float amount, int day_limit) {
    struct Account* new_node = (struct Account*) malloc(sizeof(struct Account));
    new_node->a_id  = a_id;
    new_node->c_id  = c_id;
    new_node->amount  = amount;
    new_node->day_limit  = day_limit;
    new_node->operation_count = 0;
    pthread_mutex_init(&new_node->lock, NULL);
    new_node->next = (*head_ref);
    (*head_ref) = new_node;
}

void read_file(){
    FILE * input_file;
    char line[51];

    char *item;
    int count = 0;

    input_file = fopen("input.txt","r");

    while (fgets(line,50,input_file)) {
        if(count>2) {
            Account account;

            item = strtok(line, " ");
            account.a_id = atoi(item);

            item = strtok(NULL, " ");
            account.c_id = atoi(item);

            item = strtok(NULL, " ");
            account.amount = (float) atof(item);

            item = strtok(NULL, "\n");
            account.day_limit = atoi(item);
            push(&accounts, account.a_id , account.c_id , account.amount ,account.day_limit);
        }else{
            item = strtok(line, " ");
            if(count == 0){
                nof_customers = atoi(item);
            }else if(count == 1){
                nof_tellers = atoi(item);
            } else if(count == 2){
                nof_simulation_days = atoi(item);
            }
        }
        count++;
    }

    fclose(input_file);


}

int get_accounts_size(struct Account* item) {
    struct Account* cur = item;
    int size = 0;

    while (cur != NULL) {
        ++size;
        cur = cur->next;
    }
    return size;
}
void print_list(struct Account *account) {
    while (account != NULL)
    {
        printf("a_id: %d c_id: %d amount: %f day_limit: %d operation_count: %d \n", account->a_id , account->c_id ,
               account->amount, account->day_limit,account->operation_count);
        account = account->next;
    }
    printf("Customer : %d\n", nof_customers);
    printf("Tellers : %d\n", nof_tellers);
    printf("Simulation Days : %d\n", nof_simulation_days);

}
void* customer(void* threadid){

    struct Account* account = accounts ;

    while(account!=NULL){


        pthread_mutex_lock(&account->lock);


        account->operation_count = account->operation_count+ 1;
        int id = (int)threadid;
        log_info("customer threadid: %d operation_count:%d a_id:%d ",id, account->operation_count ,account->a_id);
        pthread_mutex_unlock(&account->lock);
        account =account->next;

    }
}
void* teller(void* threadid){
    struct Account* account = accounts ;
    while(account!=NULL){

        pthread_mutex_lock(&account->lock);
        account->operation_count = account->operation_count+ 1;
        int id = (int)threadid;
        log_info("teller threadid: %d operation_count:%d a_id:%d ",id, account->operation_count ,account->a_id);
        pthread_mutex_unlock(&account->lock);

        account =account->next;
    }
}
void *clock_update(){
    while(1){
        clock_timestamp = (unsigned long) time(NULL);
        if(clock_timestamp-past_timestamp > 10) {
            simulation_day++;
            printf("simulation %d\n", simulation_day);
            past_timestamp = clock_timestamp;
        }

        if(stop_clock)
            break;

    }
}
void create_threads(){
    pthread_t customers[nof_customers];
    pthread_t tellers[nof_tellers];
    pthread_t clock_thread;
    pthread_create(&clock_thread , NULL , clock_update , NULL);
    for(int i = 0 ; i<nof_customers;i++)
        pthread_create(&customers[i], NULL ,customer , (void*)i);


    for(int j = 0 ; j<nof_tellers;j++)
        pthread_create(&tellers[j], NULL ,teller ,(void*)j);

    for(int i = 0 ; i<nof_customers ; i++)
        pthread_join(customers[i], NULL);

    for(int i = 0 ; i<nof_tellers ; i++)
        pthread_join(tellers[i], NULL);
}
int get_random_customer_id(){
    int random = rand() %account_size;
    return customer_ids[random];
}
int get_random_account_id(){
    int random = rand() % account_size;
    return account_ids[random];
}
void set_ids(){
    struct Account* account = accounts ;
    int i = 0;
    size_t nof_accounts = (size_t)account_size;
    customer_ids = (int*) calloc(nof_accounts, sizeof(int));
    account_ids = (int*) calloc(nof_accounts, sizeof(int));
    while(account!=NULL){
        account_ids[i] = account->a_id ;
        customer_ids[i] = account->c_id;
        i++;
        account =account->next;
    }
}
void init(){
    log = fopen("output.txt","w");
    log_set_fp(log);
    log_set_quiet(1);
    read_file();
    account_size = get_accounts_size(accounts);
    set_ids();
    create_threads();
   // print_list(accounts);

}
void final(){
    fclose(log);
    stop_clock=1;
}
//gcc -o a *.c -lpthread
int main(){
    init();

    final();
    return 0;
}
