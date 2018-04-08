#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "log.h"
#include <pthread.h>



typedef struct Account {
    int a_id;
    int c_id;
    float amount;
    int day_limit;
    float withdraw_total;
    int operation_count;
    pthread_mutex_t lock;
    struct Account *next;

} Account;

typedef struct Customer{
    int id;
    int action;
    int a_id;
    pthread_mutex_t lock;
    struct Customer *next;
} Customer;

typedef struct Teller{
    int id;
    int action;
    int empty ;
    int account_id;
    int customer_id ;
    int customer_account_id ;
    float amount ;
    pthread_mutex_t lock;
    struct Teller *next;
} Teller;
struct Account* accounts = NULL;
struct Teller* tellers = NULL;
struct Customer* customers = NULL;

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
void push_teller(struct Teller** head_ref, int id){
    struct Teller* new_node = (struct Teller*) malloc(sizeof(struct Teller));
    new_node->id = id ;
    new_node->action = -1 ; // action 0 read 1 write 2 add 3 sub
    new_node->empty = 1;
    pthread_mutex_init(&new_node->lock,NULL);
    new_node->next = (*head_ref);
    (*head_ref) = new_node ;
}
void push_customer(struct Customer** head_ref, int id){
    struct Customer* new_node = (struct Customer*) malloc(sizeof(struct Customer));
    new_node->id = id;
    new_node->action = 0 ; // action 0 read 1 write 2 add 3 sub default 0
    new_node->a_id = 0 ; //default 0
    pthread_mutex_init(&new_node->lock,NULL);
    new_node->next = (*head_ref);
    (*head_ref) = new_node ;
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
void print_accounts(struct Account *a) {
    Account* account = a;
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
void print_customers(struct Customer *customer) {
    while (customer != NULL)
    {
        printf("Customer: id: %d\n" ,customer->id);
        customer = customer->next;
    }

}
void print_tellers(struct Teller *teller) {
    while (teller != NULL)
    {
        printf("teller: id: %d\n" ,teller->id);
        teller = teller->next;
    }

}

Account* get_account_by_customer_id(int id){
    Account* account = accounts ;
    int n = 0 ;
    int *arr = malloc (sizeof (int) * nof_customers*3);
    while(account!=NULL){
        if(account->c_id ==id){
            arr[n] = account->a_id ;
            n++;
        }
        account=account->next;
    }
    int random = rand() % n;
    account = accounts;
    while (account!=NULL){
        if(account->a_id ==arr[random]){
            free(arr);
            return account;
        }
        account=account->next;
    }
    free(arr);
    return NULL;
}

int get_random_operation(){
    int random = rand() % 4;
    return random;
}

int get_random_account_id(int customer_id){
    Account* account = accounts;
    int i = 0 ;
    while (account!=NULL){
        if(account->c_id != customer_id){
            i++;
        }
        account=account->next;
    }
    int* ids = (int*) calloc((size_t) i, sizeof(int));
    account = accounts;
    int j =0 ;
    while (account!=NULL){
        if(account->c_id != customer_id){
            ids[j] = account->a_id;
            j++;
        }
        account=account->next;
    }
    int random = rand() % i;
    return ids[random];
}

int get_teller_id(){
    Teller* t = tellers;
    int id = -1 ;
    while(t!=NULL){
        if(t->empty==1){
            id = t->id ;
        }

        t=t->next;
    }
    return id;
}

Teller* get_teller_by_id(int id){
    Teller* t = tellers;
    while(t!=NULL){
        if(t->id == id){
            break;
        }
        t=t->next;
    }
    return t;
}

void* customer(void* threadid){
    int customer_id = (int) threadid;
    //log_info("customer id: %d" , customer_id);
    while(1){
        int operation = get_random_operation();
        int account = get_random_account_id(customer_id) ;
        Account* a = get_account_by_customer_id(customer_id);
        int own_account = a->a_id;
        int teller_id = get_teller_id();
        Teller *t ;

        if(teller_id!=-1) {

            t = get_teller_by_id(teller_id);
            if (operation == 0) {
                pthread_mutex_lock(&t->lock);
                t->customer_account_id = own_account ;
                t->customer_id = customer_id;
                t->action = 0 ;
                t->empty =0;
                pthread_mutex_unlock(&t->lock);
                //log_info("t->id %d t->empty" , t->id, t->empty);

            } else if (operation == 1 ) {
                pthread_mutex_lock(&t->lock);
                t->action = 1;
                t->account_id = account ;
                t->customer_id = customer_id;
                t->customer_account_id = own_account;
                float r = 1000.0;
                t->amount = (float) ((float)rand() / (float)(RAND_MAX) * r + 5.0);
                t->empty= 0;
                pthread_mutex_unlock(&t->lock);
                //log_info("t->id %d t->empty" , t->id, t->empty);

            } else if (operation == 2 ) {
                pthread_mutex_lock(&t->lock);
                t->action = 2;
                t->account_id = account ;
                t->customer_id = customer_id;
                t->customer_account_id = own_account;
                t->empty= 0;
                pthread_mutex_unlock(&t->lock);
                //log_info("t->id %d t->empty" , t->id, t->empty);

            } else {
                pthread_mutex_lock(&t->lock);
                t->action = 3;
                t->account_id = account ;
                t->customer_id = customer_id;
                t->customer_account_id = own_account;
                t->empty= 0;
                pthread_mutex_unlock(&t->lock);
                //log_info("t->id %d t->empty" , t->id, t->empty);
            }
        }
        if(simulation_day>=nof_simulation_days){
           // log_info("customer final %", customer_id);
            break;
        }

    }
}

Account* get_account_by_id(int id){
    Account* account = accounts ;
    while(account!=NULL){
        if(account->a_id == id){
            break;
        }
        account=account->next;
    }
    return account;
}

void* teller(void* threadid){

    int id = (int) threadid;
    Teller* t = get_teller_by_id(id);
    //log_debug("teller id: %d" , t->id);


    /*
     *  0. View
     *  1. Deposit add
     *  2. Withdraw sub
     *  3. Transfer sub to
     *
     * */
    while(1){
        if(t->empty == 0&& simulation_day>=0){

            Account* to = get_account_by_id(t->account_id);
            Account* from = get_account_by_id(t->customer_account_id);
            if(t->action==0 && from->operation_count<3){
                pthread_mutex_lock(&from->lock);

                //from->amount++;
                from->operation_count++;
                log_info("teller in %d, customer-id %d accountid %d simulationday %d view", id ,t->customer_id,t->account_id, simulation_day);
                pthread_mutex_unlock(&from->lock);

            }else if(t->action==1 && from->operation_count<3){

                pthread_mutex_lock(&from->lock);
                log_info("teller in %d, customer-id %d accountid %d  simulationdayc %d deposit", id ,t->customer_id,t->customer_account_id,simulation_day);
                //from->amount++;
                from->operation_count++;
                //operation
                pthread_mutex_unlock(&from->lock);

            }else if(t->action==2 && from->operation_count<3){

                pthread_mutex_lock(&from->lock);
                from->operation_count++;
                //from->amount--;
                log_info("teller in %d, customer-id %d accountid %d  simulationday %d withdraw", id ,t->customer_id,t->customer_account_id , simulation_day);
                pthread_mutex_unlock(&from->lock);
            }else{

                pthread_mutex_lock(&from->lock);

                if(from->operation_count<3 && to->operation_count<3){
                    log_info("teller in %d, customer-id %d accountid %d  toAccountid %d simulationday %d transfer"
                    , id ,t->customer_id,t->customer_account_id , t->account_id, simulation_day);
                    from->operation_count++;
                    to->operation_count++;
                    from->amount = from->amount-5;

                }
                pthread_mutex_unlock(&from->lock);
                pthread_mutex_lock(&to->lock);
                if(to->operation_count<3){
                    to->amount= to->amount+5;
                    to->operation_count++;
                }

                pthread_mutex_unlock(&to->lock);
            }
            t->empty = 1;
        }
        if(simulation_day>=nof_simulation_days){
            log_info("teller final %", id);
            break;
        }

    }
}
void set_account_operation_count_to_default(){
    Account* account = accounts;
    while(account!=NULL){
        account->operation_count = 0;
        account->withdraw_total = 0.0;
        account = account->next;
    }
}
void* clock_update(){
    while(1){
        clock_timestamp = (unsigned long) time(NULL);
        if(clock_timestamp-past_timestamp > 10) {
            simulation_day++;
            set_account_operation_count_to_default();
            log_info("simulation %d\n", simulation_day);
            past_timestamp = clock_timestamp;
        }

        if(stop_clock==1)
            break;

    }
}
void create_threads(){
    pthread_t customers[nof_customers];
    pthread_t tellers[nof_tellers];
    pthread_t clock_thread;
    log_debug("creating clock:");
    pthread_create(&clock_thread , NULL , clock_update, NULL);

    for(int j = 0 ; j<nof_tellers;j++)
        if( pthread_create(&tellers[j], NULL, teller, (void *) j)==0)
            log_debug("creating teller thread: %d teller id : %d ", j , j);


    for(int i = 0 ; i<nof_customers;i++)
        if(pthread_create(&customers[i], NULL, customer, (void *) customer_ids[i]) == 0)
            log_debug("creating customer thread: %d customer id : %d ", i , customer_ids[i]);

    for(int i = 0 ; i<nof_customers ; i++)
        pthread_join(customers[i], NULL);


    for(int i = 0 ; i<nof_tellers ; i++)
        pthread_join(tellers[i], NULL);

}

int get_random_customer_id(){
    int random = rand() % nof_customers;
    return customer_ids[random];
}

int is_in_customer_ids(int id){
    for(int i = 0 ; i<nof_customers ;i++){
        if(customer_ids[i]==id){
            return 1;
        }
    }
    return 0;
}

void set_ids(){
    struct Account* account = accounts ;
    int i = 0;
    size_t nof_accounts = (size_t)account_size;
    customer_ids = (int*) calloc((size_t) nof_customers, sizeof(int));
    account_ids = (int*) calloc(nof_accounts, sizeof(int));
    int j = 0;
    while(account!=NULL){
        account_ids[i] = account->a_id ;
        if(is_in_customer_ids(account->c_id)==0){
            customer_ids[j]=account->c_id;
            //push_customer(&customers,account->c_id);
            j++;
        }
        i++;
        account =account->next;
    }
    for(i = 0 ;i<nof_tellers ; i++){
        push_teller(&tellers,i);
    }
}

void init(){
    log = fopen("output.txt","w");
    //log_set_fp(log);
    //log_set_quiet(1);
    read_file();
    account_size = get_accounts_size(accounts);
    set_ids();
}
void final(){
    log_info("final");
    fclose(log);
    stop_clock=1;
}
//gcc -o a *.c -lpthread
int main(){
    init();
    create_threads();
    final();
    print_accounts(accounts);

    return 0;
}
