#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>

//definindo estrutura para registrar
typedef struct {
    char login[20];
    char senha[40];
} registro; registro rg[30];

//criar linha separadora
void linha(){
	printf("\n");
	for(int x=0; x<50; x++){
		printf("\x1b[1;34m=\x1b[0m");
	}
	printf("\n");
}

//criar linha separadora fina
void linhafina(){
	printf("\n");
	for(int x=0;x<50;x++){
		printf("\x1b[1;32m-\x1b[0m");	
	}
	printf("\n");
}

//deletar ou alterar registros
void alterar(int h, int *p,FILE *log){
    int esc;
    printf("\x1b[1;33m(1) Alterar senha | (2) Deletar registro\x1b[0m\nEscolha: ");
    scanf("%d", &esc);
    
    if(esc == 1) {
        char senha2[40], y;
        int x, z;
        
        printf("Digite a nova senha (mínimo 9 caracteres): ");
        do {
            scanf("%s", senha2);
        } while(strlen(senha2) <= 8);

        for(x=0; x<strlen(senha2); x++){
            y = senha2[x];
            z = (int)y;
            y = (char)(z + 3);
            rg[h].senha[x] = y;
        }
        printf("\n\x1b[1;32mSenha alterada com sucesso!\x1b[0m\n");
        
    } else if(esc == 2) {
    	int i;
        for(i = h; i < *p - 1; i++) {
            rg[i] = rg[i + 1];
        }
        
        (*p)--;
        printf("\n\x1b[1;32mRegistro deletado com sucesso!\x1b[0m\n");
    }
    for (int r=0; r<*p; r++){
        fprintf(log, "%s %s\n", rg[r].login, rg[r].senha);
    }
    system("pause");
}

//fazer login
void login(int total){
	int x,z,dc=0;;
	
	do{
		char lg[20]="",s[40]="",senha[40]="",l[20]="",y;
		
		system("cls");
		linha();
		printf("\x1b[1;36m- - -LOGAR - - -\x1b[0m");
		linha();

		printf("Login: ");
		scanf("%s",lg);
		printf("Senha: ");
		scanf("%s",senha);
		
		dc = -1;
		//verificando se login bate com a senha
		for(int a=0;a<total;a++){ 
			char sn[40];
			strcpy(sn,rg[a].senha);
			//descriptografar senha
			for(x=0;x<strlen(sn);x++){ 
				y =sn[x];
		        z=(int)y;
		        y=(char)(z-3);
		        s[x]=y;
			}	
			s[strlen(sn)] = '\0';
			
			if(strcmp(s, senha)==0 && strcmp(rg[a].login, lg)==0){
				printf("\n\x1b[1;32m-Login feito com Sucesso -\x1b[0m\n");
				dc=-2;
				break;
			}
		} 

		if(dc==-2){
			system("pause");
			system("cls");	
		} else {
			printf("\n\x1b[1;31m(!)Login ou senha incorretos(!)\x1b[0m\n \n");
			printf("\x1b[1;33mDigite - (1)para continuar | (2)para voltar ao início\x1b[0m\n");
			scanf("%d",&dc);
			if(dc==1){
				dc=0;
			} else {
				system("cls");
			}
		}
	} while(dc==0);
}

//registrar conta
void registrar(int *rept, FILE *log){
	char login[20],senha[40]="",y,s[40];
	int login_ex,x,z;
	
	linha();
	printf("\x1b[1;36m- - -REGISTRAR - - -\x1b[0m");
	linha();
	
	do{ //registrando login
		login_ex = 0;
		
		printf("Login (mínimo 3 caracteres): ");
		scanf("%s",login);
		
		for(x=0;x<=*rept;x++){ 	//verificar se existe outro login com mesmo nome
			if(strcmp(rg[x].login,login)==0){
				printf("\n\x1b[1;31m(!)Login ja existente(!)\x1b[0m\n \n\n");
				login_ex = 1;
			}
		}
	} while(login_ex == 1 || strlen(login)<3);
	strcpy(rg[*rept].login,login);
		
	do{ //registrando senha
		printf("Senha (mínimo 9 caracteres): ");
		scanf("%s",s);
	} while(strlen(s)<=8);
	//criptografar senha
	for(x=0;x<strlen(s);x++){
		y =s[x];
        z=(int)y;
        y=(char)(z+3);
        
        senha[x]=y;
	}
	printf("\n\x1b[1;32m-Registrado com Sucesso -\x1b[0m\n");
	system("pause");
	system("cls");

	//Salvando no TXT
	strcpy(rg[*rept].senha,senha);
	fprintf(log, "%s %s\n", rg[*rept].login,rg[*rept].senha);
	*rept+=1;
}

//lista de Registros
void lista_rg(int *o){
	int y, z;
	
	linha();
	printf("\x1b[1;36m- - -LISTA DE REGISTROS - - -\x1b[0m");
	linha();
	
	if(*o == 0){ //verificando se ja existem registros
		printf("\n\x1b[1;31mNão existem registros até o momento.\x1b[0m\n");
	} else {
		for(y=0;y<*o;y++){
			printf("\x1b[1;33mLogin\x1b[0m \x1b[1;37m%d\x1b[0m - %s \n\x1b[1;33mSenha\x1b[0m \x1b[1;37m%d\x1b[0m - %s",y+1,rg[y].login,y+1,rg[y].senha);
			linhafina();
		}
	}
	
	linha();
}

int main(){
	setlocale(LC_ALL, "Portuguese");
	//variaveis globais
	int ida=0,x=0,z=0;
	char temp[50],string[x],loginn[20],senha[40];
	
	FILE *log = fopen("log.txt", "a+");
	if (log == NULL) {
        printf("Erro ao abrir o arquivo");
        return 1;
    }
    //ler log.txt e reamarzendo dados
	while (fscanf(log, "%s %s", loginn, senha) != EOF) {
        strcpy(rg[ida].login, loginn);
        strcpy(rg[ida].senha, senha);
        ida++;
    }

    fclose(log);

	//menu inicial
	do{
		do{
			linha();
			printf("\x1b[1;36m-@- BEM-VINDO -@-\x1b[0m");
			linha();
			printf("\nO que deseja fazer hoje ?\n");
			printf("\n(1)Fazer login\n(2)Registrar nova conta\n(3)Ver lista de registros\n(4)Sair\n");
			scanf("%d",&x);
			if(x>4 || x<1){
				system("cls");
			}
		} while(x>4 && x<1);
		
		switch(x){
			case 1://realizar login
				system("cls");
				login(ida);
				x=0;
			break;
			
			case 2: //registrar conta
				log = fopen("log.txt", "a");
				system("cls");
				registrar(&ida,log);
				fclose(log);
				x=0;
			break;
			
			case 3: //lista de registros
				system("cls");
				lista_rg(&ida);
				printf("(0 para sair) Digite o numero do registro a ser alterado: ");
				scanf("%d", &z);
				if(z>0 && z<=ida) {
				log = fopen("log.txt", "w");
				alterar(z - 1,&ida,log);
				fclose(log);
				}
				system("cls");
				x=0;
			break;
			
			case 4: //fechar programa
				linha();
				printf("Saindo do programa...");
			break;
		}
	} while(x!=4);

	return 0;
}
