#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio2.h>
#include <string.h>

#define TF 20

struct Data{
	int dia,mes,ano;
};
struct Hora{
	int hora,min;
};

struct User{
    char usuario[TF];
    char senha[TF];
    char status; // A - ATIVO ; I - INATIVO; S - STAFF;
    int qntdConsultasON=0;
};

struct Consulta{
	int pos=0;
	Data dtConsulta;
	Hora hrConsulta;
	char usuario[TF];
	char status; // A - ATIVO; I - INATIVO;
};

void Login();
void Cadastro();
void Admin();
void startFiles();
int buscaUser(char user[],FILE Ptr);
void MenuUser();

void startFiles(){
    FILE *User = fopen("Users.dat","ab");
    FILE *Cons = fopen("Consultas.dat","ab");
    fclose(Cons);
    fclose(User);
    fclose(Adm);
}
void startAdmin(){
	User Reg;
	FILE *Adm = fopen("secrets.dat","wb");
	
	strcpy(Reg.usuario,"admin");
	strcpy(Reg.senha,"admin");
	Reg.status='S';
	
	fwrite(&Reg,sizeof(User),1,Adm);
	fclose(Adm);
}

int buscaUser(char user[],FILE *Ptr){
    User Reg;

    fseek(Ptr,0,SEEK_SET);
    while(!feof(Ptr) && strcmp(Reg.usuario,user)!=0){
        fread(&Reg,sizeof(User),1,Ptr);
    }
    if(!feof(Ptr)){
        return ftell(Ptr) - sizeof(User);
    }
    else return -1;
}

int excluiConsulta(int pos, FILE *Ptr){
	Consulta Reg;
	
	fseek(Ptr,pos*sizeof(Ptr),SEEK_SET);
	fread(&Reg,sizeof(Consulta),1,Ptr);
	Reg.status = 'I';
	fseek(Ptr,pos*sizeof(Ptr),SEEK_SET);
	fwrite(&Reg,sizeof(Consulta),1,Ptr);
}

void Menu(){
	printf("[1] - LOGIN\n");
	printf("[2] - CADASTRO\n");
	printf("[3] - ADMIN\n");
	printf("[ESC] - SAIR\n");
}

void MenuUser(){
    printf("[1] - Marcar consulta\n");
    printf("[2] - Verificar consultas\n");
    printf("[3] - Cancelar consultas\n");
    printf("[TAB] - SAIR\n");
}

void MenuAdmin(){
	printf("[1] - Visualizar Usuarios\n");
    printf("[2] - Visualizar consultas\n");
    printf("[TAB] - SAIR\n");
}

void MarcarConsulta(User Reg,FILE *PtrUser){
	FILE *PtrConsultas = fopen("Consultas.dat","ab");
	Consulta RegConsulta;
	clrscr();
	
	printf("Qual a data da consultas? [DD/MM/AAAA]\n");
	scanf("%d %d %d",&RegConsulta.dtConsulta.dia,&RegConsulta.dtConsulta.mes,&RegConsulta.dtConsulta.ano);
	printf("Qual a hora desejada?[HH:MM]\n");
	scanf("%d %d",&RegConsulta.hrConsulta.hora,&RegConsulta.hrConsulta.min);
	RegConsulta.status='A';
	strcpy(RegConsulta.usuario,Reg.usuario);
	
	fseek(PtrConsultas,0,SEEK_END);
	RegConsulta.pos=ftell(PtrConsultas)/sizeof(Consulta);
	fwrite(&RegConsulta,sizeof(Consulta),1,PtrConsultas);
	
	Reg.qntdConsultasON++;
	fseek(PtrUser,ftell(PtrUser)-sizeof(User),SEEK_SET);
	fwrite(&Reg,sizeof(User),1,PtrUser);
	
	printf("Consulta agendada!\n");
	fclose(PtrConsultas);
	getch();	
}

void VerificarConsulta(char usuario[]){
	clrscr();
	FILE *PtrConsultas = fopen("Consultas.dat","rb");
	int i=0;
	Consulta Reg;
	
	printf("=> Consultas agendadas de: %s\n",usuario);
	fread(&Reg,sizeof(Consulta),1,PtrConsultas);
	while(!feof(PtrConsultas)){
		fread(&Reg,sizeof(Consulta),1,PtrConsultas);
		if(strcmp(Reg.usuario,usuario)==0 && Reg.status=='A'){
			i++;
			printf("%d) %d/%d/%d - %d:%d\n",Reg.pos+1,Reg.dtConsulta.dia,Reg.dtConsulta.mes,Reg.dtConsulta.ano,Reg.hrConsulta.hora,Reg.hrConsulta.min);
		}
	}
	if(i==0){
		printf("Nao ha consultas para %s\n",usuario);
	}
	getch();
	fclose(PtrConsultas);
}

void CancelarConsulta(User Reg,FILE *PtrUser){
	clrscr();
	FILE *PtrConsultas = fopen("Consultas.dat","ab");
	Consulta RegConsulta;
	int pos;
	
	printf("Qual consulta deseja cancelar? [EX: 1]\n");
	scanf("%d",&pos);
	pos--;
	printf("%d",pos);
	getch();
	
	excluiConsulta(pos,PtrConsultas);
	
	fclose(PtrConsultas);
}

void exibirUsuarios(){
	FILE *Ptr = fopen("Users.dat","rb");
	User Reg;
	
	fread(&Reg,sizeof(User),1,Ptr);
	while(!feof(Ptr)){
		printf("%s - [%d] - %c",Reg.usuario,Reg.qntdConsultasON,Reg.status);
		fread(&Reg,sizeof(User),1,Ptr);
	}
	
	fclose(Ptr);
}

void admConsultas(){
	FILE *Ptr = fopen("Consultas.dat","rb");
	Consulta Reg;
	
	fread(&Reg,sizeof(Consulta),1,Ptr);
	while(!feof(Ptr)){
		printf("%s - [%d/%d/%d - %d:%d] %c",Reg.usuario,Reg.dtConsulta.dia,Reg.dtConsulta.mes,Reg.dtConsulta.ano,Reg.hrConsulta.hora,Reg.hrConsulta.min,Reg.status);
		fread(&Reg,sizeof(Consulta),1,Ptr);
	}
	
	fclose(Ptr);
}

void Login(){
    FILE *PtrUser = fopen("Users.dat","rb");
    clrscr();

    User Reg;
    char usuario[TF],senha[TF];
    int esc,pos;

    printf("USERNAME: ");
    fflush(stdin);
    gets(usuario);
    printf("SENHA: ");
    fflush(stdin);
    gets(senha);

    pos=buscaUser(usuario,PtrUser);
    
    if(pos!=-1){
	   	fseek(PtrUser,pos,SEEK_SET);
        fread(&Reg,sizeof(User),1,PtrUser);
    	if(Reg.status=='A'){
    		if(strcmp(Reg.senha,senha)==0){
            do{
            	clrscr();
            	MenuUser();
            	esc=getch();
                switch (esc) {
                    case '1': MarcarConsulta(Reg,PtrUser);break;
                    case '2': VerificarConsulta(Reg.usuario);break;
                    case '3': CancelarConsulta(Reg,PtrUser);break;
                }
            }while(esc!=9);
	    	}
	    	else{
	    		printf("SENHA INVALIDA!\n");
	    	}
    	}
    	else{
    		printf("Usuario inativo!\n");
        	getch();
    	}
	}
    else{
        printf("Usuario nao encontrado!\n");
        getch();
    }
    fclose(PtrUser);
}

void Cadastro(){
	FILE *PtrUser = fopen("Users.dat","ab+");
	User Reg;
	clrscr();
	
	printf("=== Cadastro ===\n\n");
	printf("USUARIO: ");
	fflush(stdin);
	gets(Reg.usuario);
	printf("SENHA: ");
	fflush(stdin);
	gets(Reg.senha);
	
	if(buscaUser(Reg.usuario,PtrUser)!=-1){
		printf("Usuario ja existente!\n");
	}
	else{
		Reg.status = 'A';
		fwrite(&Reg,sizeof(User),1,PtrUser);
		printf("Usuario cadastrado com sucesso!\n");
	}
	getch();
	
	fclose(PtrUser);
}

void Admin(){
    FILE *PtrAdmin = fopen("secrets.dat","rb");
    clrscr();

    User Reg;
    char usuario[TF],senha[TF];
    int esc,pos;

    printf("USERNAME: ");
    fflush(stdin);
    gets(usuario);
    printf("SENHA: ");
    fflush(stdin);
    gets(senha);

    pos=buscaUser(usuario,PtrAdmin);
    
    if(pos!=-1){
	   	fseek(PtrAdmin,pos,SEEK_SET);
        fread(&Reg,sizeof(User),1,PtrAdmin);
    	if(Reg.status=='S'){
    		if(strcmp(Reg.senha,senha)==0){
	            do{
	            	clrscr();
	            	MenuAdmin();
	            	esc=getch();
	                switch (esc) {
	                    case '1': exibirUsuarios();break;
	                    case '2': admConsultas();break;
	                }
	            }while(esc!=9);
	    	}
	    	else{
	    		printf("SENHA INVALIDA!\n");
	    	}
    	}
    	else{
    		printf("Permissoes administrativas negadas!\n");
        	getch();
    	}
	}
    else{
        printf("Usuario nao encontrado!\n");
        getch();
    }
    fclose(PtrUser);
}


void excluiCadastrosInativos(){
	
}

void excluiConsultasInativas(){
	
}

int main(void){
    startFiles();
    startAdmin();

    int esc;

    do {
        clrscr();
        Menu();
        esc = toupper(getch());
        switch (esc) {
            case '1': Login(); break;
            case '2': Cadastro(); break;
            case '3': Admin(); break;
        }
    } while (esc != 27);

    return 0;
}

