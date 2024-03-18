# Github en Ubuntu

sudo apt-get install git

## Configuración inicial de Github

Una vez que la instalación se ha completado con éxito, el siguiente paso es configurar los detalles de configuración del usuario GitHub.

git config –global user.name “nombre_usuario”
git config –global user.email “email_id”

## Creación de un repositorio local

Lo primero es crear una carpeta en tu computadora, que servirá como un repositorio local.

git init GitHubRepoTest

Este comando crea la carpeta GitHubRepoTest . A su vez, la sub-carpeta .init hace que GitHubRepoTest sea reconocido como un repositorio local de Git. Si se crea el repositorio con éxito, aparecerá una línea similar a la siguiente:

Initialized empty Git repository in /home/tu_usuario/ GitHubRepoTest/.git/

cd GitHubRepoTest

## Creación de un archivo README para describir el repositorio

El archivo README se utiliza generalmente para describir lo que el repositorio contiene o lo que el proyecto se trata. Para crear uno, basta ejecutar:

gedit README

Una vez que hayas ingresado la descripción del repositorio, no olvides guardar los cambios.
Adición de los archivos del repositorio a un índice

Este es un paso importante. Antes de poder subir los cambios a Github u otro servidor compatible con Git, hay que indexar todos los archivos contenidos en el repositorio local. Este índice contendrá los archivos nuevos así como los cambios a los archivos existentes en el repositorio local.

En nuestro caso, nuestro repositorio local ya contiene un nuevo archivo: el README. Por lo tanto, vamos a crear otro archivo con un programa C simple y al cual llamaremos ejemplo.c. Los contenidos del mismo serán:

#include <stdio.h>

int main()
{
printf(“Prueba”);
return 0;
}

Así que, ahora tenemos 2 archivos en nuestro repositorio local: README y prueba.c.

El siguiente paso es agregar estos archivos al índice:

git add README
git add prueba.c

El comando «git add» se puede utilizar para agregar cualquier número de archivos y carpetas al índice. Para agregar todos los cambios, sin especificar el nombre de los archivos, es posible ejecutar «git add .» (con un punto al final). Guardar los cambios realizados en el índice. Una vez añadidos todos los archivos, es posible dejar un registro de estos cambios haciendo lo que en la jerga se llama un «commit». Esto significa que ya se ha terminado de agregar o modificar archivos y que los cambios pueden ser subidos al repositorio remoto de Github. Para ello, hay que ejecutar el siguiente comando:

git commit -m “mensaje”

«mensaje» puede ser cualquier mensaje que describa brevemente los cambios en cuestión, por ejemplo: «agregué tal funcionalidad» o «corregí tal cosa», etc.
Creación de un repositorio en GitHub

El nombre del repositorio debe ser el mismo que el repositorio del sistema local. En este caso, será « GitHubRepoTest ». Para ello, antes que nada, hay que iniciar sesión en Github. Luego, hay que hacer clic en el signo más (+) en la esquina superior derecha de la página y seleccionar la opción «crear nuevo repositorio». Finalmente, hay que rellenar los datos y hacer clic en el botón «crear repositorio».

Una vez hecho esto se creará el repositorio y será posible subir el contenido del repositorio local en el repositorio GitHub. Para conectarse al repositorio remoto en GitHub hay que ejecutar el comando:

git remote add origin https://github.com/user_name/ GitHubRepoTest.git

## Pasos resumidos

git status

git checkout -b pruebas (para cambiar de branch a pruebas)

git push origin pruebas (para relacionar branch local con el remoto pruebas)

## Para agregar archivos nuevos o cambios:

git init
git remote add origin https://github.com/directorioUsuarioGit/IoT01.git
git add .
git commit -m "Initial commit"
git push -u origin pruebas

git init /home/pi/Devel/GitRepo/IoT01

git remote add origin https://github.com/directorioUsuarioGit/IoT01.git

git checkout -b nombreBranch (para seleccionar branch) -b si no existe

git pull origin nombreBranch

git config –list (lista configuración)

git config –global user.name “usuarioGit”

git config –global user.email “mail del usuario GitHub”

git add .

git commit – m “ok”

git push origin nombreBranch

…

git pull origin nombreBranch > (para traer archivos)

git branch [-a] > muestra el branch activo
