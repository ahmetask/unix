#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "log.h"
#include <pthread.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
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
    int nof_operation_performed;
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
    int nof_operation_performed;
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

FILE* log_file = NULL;
FILE* log_output = NULL;


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
    new_node->action = -1;
    new_node->empty = 1;
    new_node->nof_operation_performed = 0;
    pthread_mutex_init(&new_node->lock,NULL);
    new_node->next = (*head_ref);
    (*head_ref) = new_node ;
}
void push_customer(struct Customer** head_ref, int id){
    struct Customer* new_node = (struct Customer*) malloc(sizeof(struct Customer));
    new_node->id = id ;
    new_node->nof_operation_performed = 0;
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
        log_info("a_id: %d c_id: %d amount: %f day_limit: %d operation_count: %d", account->a_id , account->c_id ,
               account->amount, account->day_limit,account->operation_count);
        account = account->next;
    }

}
void print_teller(){
    Teller* teller = tellers;
    while(teller!=NULL){
        log_info("Teller:%d Number of Operation Performed:%d", teller->id , teller->nof_operation_performed);
        fprintf(log_output,"Teller:%d #:%d\n", teller->id, teller->nof_operation_performed);
        teller=teller->next;
    }
}
void print_customer(){
    Customer* customer = customers;
    while(customer!=NULL){
        log_info("Customer:%d Number of Operation Performed:%d", customer->id , customer->nof_operation_performed);
        fprintf(log_output,"Customer:%d #:%d\n", customer->id , customer->nof_operation_performed);
        customer=customer->next;
    }
}
float get_current_balance_customer(int customer_id){
    Account* account = accounts;
    float balance = 0;
    while(account!=NULL){
        if(account->c_id == customer_id){
            balance+=account->amount;
        }
        account = account->next;
    }
    return balance;
}
void print_current_balance(){
    for(int i=0; i<nof_customers ; i++){
        log_info("customer:%d balance:%f", customer_ids[i] , get_current_balance_customer(customer_ids[i]));
        fprintf(log_output,"Customer:%d Balance:%.2f\n",customer_ids[i] ,get_current_balance_customer(customer_ids[i]));
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
    int id = ids[random];
    free(ids);
    return id;
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
float get_total_cash(){
    Account* account = accounts;
    float total = 0.0;
    while(account!=NULL){
        total=total+account->amount;
        account=account->next;
    }
    return total;
};
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
Customer* get_customer_by_id(int id){
    Customer* t = customers;
    while(t!=NULL){
        if(t->id == id){
            break;
        }
        t=t->next;
    }
    return t;
}
void* customer(void* threadid){
    int customer_id = (int) (intptr_t) threadid;
    while(1){
        if(simulation_day>=nof_simulation_days){
            break;
        }
        int operation = get_random_operation();
        int account = get_random_account_id(customer_id) ;
        Account* a = get_account_by_customer_id(customer_id);
        int own_account = a->a_id;
        int teller_id = get_teller_id();
        Teller *t ;

        if(teller_id!=-1) {

            t = get_teller_by_id(teller_id);
            if (operation == 0 ) {
                pthread_mutex_lock(&t->lock);
                t->customer_account_id = own_account ;
                t->customer_id = customer_id;
                t->action = 0 ;
                t->amount=0;
                t->empty =0;
                //pthread_mutex_unlock(&t->lock);
            } else if (operation == 1) {
                pthread_mutex_lock(&t->lock);
                t->action = 1;
                t->account_id = account ;
                t->customer_id = customer_id;
                t->customer_account_id = own_account;
                int j = ((rand() % 200 +1))*5;
                t->amount = j;
                t->empty= 0;
                //pthread_mutex_unlock(&t->lock);

            } else if (operation == 2 ) {
                pthread_mutex_lock(&t->lock);
                t->action = 2;
                t->account_id = account ;
                t->customer_id = customer_id;
                t->customer_account_id = own_account;
                int j = ((rand() % 40 +1))*5; // assume min daylimit 200
                t->amount = j;
                t->empty= 0;
                //pthread_mutex_unlock(&t->lock);

            } else if(operation==3 ){
                pthread_mutex_lock(&t->lock);
                t->action = 3;
                t->account_id = account ;

                int j = rand() % 1000 +1;
                t->amount = j;
                t->customer_id = customer_id;
                t->customer_account_id = own_account;
                t->empty= 0;
                //pthread_mutex_unlock(&t->lock);
            }
        }


    }
}
Account* get_account_by_id(int id) {
    Account *account = accounts;
    while (account != NULL) {
        if (account->a_id == id) {
            break;
        }
        account = account->next;
    }
    return account;
}
void* teller(void* threadid){

    int id = (int) (intptr_t) threadid;
    Teller* t = get_teller_by_id(id);
    /*
     *  0. View
     *  1. Deposit add
     *  2. Withdraw sub
     *  3. Transfer sub to
     *
     * */
    char op_view[9]     = "View    ";
    char op_deposit[9]  = "Deposit ";
    char op_withdraw[9] = "Withdraw";
    char op_transfer[9] = "Transfer";
    int pre_exit = 0;
    while(1){
        if(t->empty == 0 && simulation_day>=0){
            Account* to = get_account_by_id(t->account_id);
            Account* from  = get_account_by_id(t->customer_account_id);
            Customer* customer = get_customer_by_id(t->customer_id);
            if(t->action==0 ) {
                pthread_mutex_lock(&from->lock);

                if (from->operation_count < 3 && simulation_day<nof_simulation_days) {

                    log_info("Before A_ID:%d C_ID:%d T_ID:%d Operation:%s Amount:%f Simulation_Day:%d", from->a_id,
                             t->customer_id, t->id, op_view, from->amount, simulation_day);
                    t->nof_operation_performed++;
                    from->operation_count++;
                    customer->nof_operation_performed++;

                    fprintf(log_output,"%d\t%d\t%d\t%s\t%.2f\t%d\n" , from->a_id ,t->customer_id ,t->id, op_view , from->amount ,simulation_day);

                }
                pthread_mutex_unlock(&from->lock);
            }else if(t->action==1 ){
                pthread_mutex_lock(&from->lock);

                if(from->operation_count<3  && simulation_day<nof_simulation_days) {

                    log_info("Before A_ID:%d C_ID:%d T_ID:%d Operation:%s Account_Amount:%f Amount:%f Simulation_Day:%d",from->a_id,t->customer_id,t->id,op_deposit,from->amount,t->amount , simulation_day);
                    t->nof_operation_performed++;
                    from->operation_count++;
                    customer->nof_operation_performed++;
                    from->amount = from->amount + t->amount;
                    log_info("After A_ID:%d C_ID:%d T_ID:%d Operation:%s Account_Amount:%f Amount:%f Simulation_Day:%d",from->a_id,t->customer_id,t->id,op_deposit,from->amount,t->amount , simulation_day);
                    fprintf(log_output,"%d\t%d\t%d\t%s\t%.2f\t%d\n" , from->a_id ,t->customer_id ,t->id, op_deposit , from->amount ,simulation_day);

                }
                pthread_mutex_unlock(&from->lock);

            }else if(t->action==2){

                pthread_mutex_lock(&from->lock);

                if(from->operation_count<3  && simulation_day<nof_simulation_days) {

                    if (from->withdraw_total + t->amount < from->day_limit && t->amount<from->amount) {
                        log_info("Before A_ID:%d C_ID:%d T_ID:%d Operation:%s Account_Amount:%f Amount:%f Simulation_Day:%d",from->a_id,t->customer_id,t->id,op_withdraw,from->amount,t->amount,simulation_day);
                        from->amount = from->amount - t->amount;
                        from->withdraw_total += t->amount;
                        from->operation_count++;
                        t->nof_operation_performed++;
                        customer->nof_operation_performed++;
                        log_info("After A_ID:%d C_ID:%d T_ID:%d Operation:%s Account_Amount:%f Amount:%f Simulation_Day:%d",from->a_id,t->customer_id,t->id,op_withdraw,from->amount,t->amount,simulation_day);
                        fprintf(log_output,"%d\t%d\t%d\t%s\t%.2f\t%d\n" , from->a_id ,t->customer_id ,t->id, op_withdraw , from->amount,simulation_day);

                    } else {
                        log_info("A_ID:%d C_ID:%d T_ID:%d Operation:%s-Error Account_Amount:%f Amount:%f Day_limit: %d Simulation_Day:%d",from->a_id,t->customer_id,t->id,op_withdraw,from->amount,t->amount,from->day_limit,simulation_day);
                    }
                }
                pthread_mutex_unlock(&from->lock);
            }else if(t->action==3) {

                pthread_mutex_lock(&from->lock);
                int canTransfer = 0;
                if (from->operation_count < 3  && simulation_day<nof_simulation_days) {

                    if (from->amount > t->amount) {
                        log_info("Before A_ID:%d C_ID:%d T_ID:%d Operation:%s Account_Amount:%f Amount:%f Day:%d To: %d",
                                 from->a_id, t->customer_id, t->id, op_transfer, from->amount, t->amount,
                                 simulation_day,to->a_id);
                        t->nof_operation_performed++;
                        customer->nof_operation_performed++;
                        from->amount = from->amount - t->amount;
                        from->operation_count++;
                        canTransfer = 1;

                        log_info("After A_ID:%d C_ID:%d T_ID:%d Operation:%s Account_Amount:%f Amount:%f Day:%d To:%d",
                                 from->a_id, t->customer_id, t->id, op_transfer, from->amount, t->amount,
                                 simulation_day,to->a_id);

                        fprintf(log_output,"%d\t%d\t%d\t%s\t%.2f\t%d\n" , from->a_id ,t->customer_id ,t->id, op_transfer ,from->amount ,simulation_day);
                    } else {
                        log_info(
                                "A_ID:%d C_ID:%d T_ID:%d Operation:%s-Error Account_Amount:%f Amount:%f Day:%d To: %d",
                                from->a_id, t->customer_id, t->id, op_transfer, from->amount, t->amount,
                                simulation_day,to->a_id);
                    }

                }
                pthread_mutex_unlock(&from->lock);
                if (canTransfer == 1) {
                    pthread_mutex_lock(&to->lock);
                    to->amount = to->amount + t->amount;
                    pthread_mutex_unlock(&to->lock);
                }
            }

            t->empty = 1;
            pthread_mutex_unlock(&t->lock);
        }

        if(simulation_day>=nof_simulation_days){
            t->empty =0;
            break;
        }
    }


    log_info("t:%d  empty: %d", t->id ,t->empty);
    if(pthread_mutex_trylock(&t->lock)!=0){
        log_info("t:%d locked empty: %d", t->id ,t->empty);
        t->empty= 0;
        pthread_mutex_unlock(&t->lock);
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
        if(clock_timestamp-past_timestamp > 9) {
            simulation_day++;
            log_info("simulation_day : %d" ,simulation_day);
            if(simulation_day<nof_simulation_days)
                set_account_operation_count_to_default();
            past_timestamp = clock_timestamp;
        }
        if(simulation_day>=nof_simulation_days){
            break;
        }
        if(stop_clock==1)
            break;
    }
}
void create_threads(){
    pthread_t customers[nof_customers];
    pthread_t tellers[nof_tellers];
    pthread_t clock_thread;
    pthread_create(&clock_thread , NULL , (void *(*)(void *)) clock_update, NULL);

    for(int j = 0 ; j<nof_tellers;j++)
        if( pthread_create(&tellers[j], NULL, teller, (void *)(intptr_t) j)!=0)
            log_debug("creating teller thread: %d teller id : %d ", j , j);


    for(int i = 0 ; i<nof_customers;i++)
        if(pthread_create(&customers[i], NULL, customer, (void *)(intptr_t) customer_ids[i]) != 0)
            log_debug("creating customer thread: %d customer id : %d ", i , customer_ids[i]);

    for(int i = 0 ; i<nof_customers ; i++)
        pthread_join(customers[i], NULL);


    for(int i = 0 ; i<nof_tellers ; i++)
        pthread_join(tellers[i], NULL);

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
            push_customer(&customers,customer_ids[j]);
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
    srand((unsigned int) time(NULL));
    log_file = fopen("output.txt","w");

    log_output = fopen ("log.txt", "w");
    log_set_fp(log_file);
    //log_set_quiet(1);
    read_file();
    account_size = get_accounts_size(accounts);
    set_ids();

}
void final(){
    fclose(log_file);
    fclose(log_output);
    stop_clock=1;
}
//gcc -o a *.c -lpthread
int main(){
    init();
    create_threads();
   // print_accounts(accounts);
    print_current_balance();
    print_customer();
    print_teller();
    log_info("Total cash:%f",get_total_cash());
    fprintf(log_output,"Total cash:%.2f", get_total_cash());
    final();
    return 0;
}
