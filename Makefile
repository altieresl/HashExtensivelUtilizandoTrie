#### Definições do compilador e da ferramenta compartilhadas por todos os destinos de build #####
CCC = g++
CXX = g++
BASICOPTS = -O
CCFLAGS = -std=c++0x $(BASICOPTS)
CXXFLAGS = -std=c++0x $(BASICOPTS)
CCADMIN = 


# Definir os diretórios de destino.
TARGETDIR_executavel.out=.


all: executavel.out

## Destino: executavel.out
OBJS_executavel.out =  \
	$(TARGETDIR_executavel.out)/Principal.o \
	$(TARGETDIR_executavel.out)/TabelaHash.o
USERLIBS_executavel.out = $(SYSLIBS_executavel.out) 
DEPLIBS_executavel.out =  
LDLIBS_executavel.out = $(USERLIBS_executavel.out)


# Link ou archive
$(TARGETDIR_executavel.out)/executavel.out: $(TARGETDIR_executavel.out) $(OBJS_executavel.out) $(DEPLIBS_executavel.out)
	$(LINK.cc) $(CCFLAGS_executavel.out) $(CPPFLAGS_executavel.out) -o $@ $(OBJS_executavel.out) $(LDLIBS_executavel.out)


# Compilar arquivos de código-fonte nos arquivos .o
$(TARGETDIR_executavel.out)/Principal.o: $(TARGETDIR_executavel.out) Principal.cpp
	$(COMPILE.cc) $(CCFLAGS_executavel.out) $(CPPFLAGS_executavel.out) -o $@ Principal.cpp

$(TARGETDIR_executavel.out)/TabelaHash.o: $(TARGETDIR_executavel.out) TabelaHash.cpp
	$(COMPILE.cc) $(CCFLAGS_executavel.out) $(CPPFLAGS_executavel.out) -o $@ TabelaHash.cpp



#### Limpar o destino exclui todos os arquivos gerados ####
clean:
	rm -f \
		$(TARGETDIR_executavel.out)/executavel.out \
		$(TARGETDIR_executavel.out)/Principal.o \
		$(TARGETDIR_executavel.out)/TabelaHash.o
# Ativar verificação de dependências
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Linux

