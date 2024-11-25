# Como compilar o código?

Segue abaixo instruções de como compilar o código do repositório

## WSL (Ubuntu)

Para instalar uma distro Linux no Windows, usaremos o WSL

Para instalar a distro do Ubuntu, basta ir em um terminal CMD do Windows e digitar

`wsl --set-default-version 2`

e

`wsl --install -d Ubuntu`

## Linux (Ubuntu)

Após instalar o sistema operacional, execute o comando abaixo para atualizar todos os pacotes e dependencias do Ubuntu:

`sudo apt update && sudo apt upgrade`

Feito isso vamos precisar instalar o compilador e a ferramenta de build, para isso execute o seguinte comando:

`sudo apt install gcc make gdb`

Após instalar as ferramentas de compilação, vá até a raiz do projeto e execute o comando `make`

Isso irá gerar um executável na pasta `bin/` chamado `main`

Para executá-lo você pode executar:

`./bin/main`

ou você pode rodar pelo VisualStudioCode pelo botão de run ou pressionando F5
