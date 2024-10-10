# MiniShell

Ce projet consistait a réaliser une version miniature d'un Shell UNIX capable d'utiliser des commandes simple, commandes internes, gérer des redirections et de gérer des des variables d'environnement.
## Fonctionnalités Demandé

- Les commandes intégrés comme $cd [dir] ou $exit [n].
- Des commandes simples en 'premier plan' ou 'arriere plan': 
```Shell
$commande ou $commande & ou $commande1 ; commande2 ; commande3 & commande4 ; ... .
```
- Les redirections entre-sortie: 
```Shell
$commande > file.output $commande 2> file.error $commande < file.input $commande >> file.appout $commande 2>> file.apperr $commande >&2 $commande 2>&1 $commande1 | commande2 | commande3 | ..
```
- De gérer les variables d’environnement:
```Shell 
$export VAR="ma variable" $echo $VAR $unset VAR 
```
- De gérer les opérateurs !, && et || comme les gère le bash.
## Run Locally

Clone the project

```bash
  git clone https://github.com/ChairHomard45/MiniShell.git
```

Aller dans le dossier du shell

```bash
  cd MiniShell/shell
```

Compiler le minishell

```bash
    make
```

Lancer le minishell

```bash
  ./minishell
```


## Usage/Examples

Liste de Commandes tester:
```bash
cd src
```
```bash
cd ..
```
```bash
exit
```
```bash
ls
```
```bash
sleep 5 &
```
```bash
ls; echo "Bonjour"; ls -l; 
```
```bash
ls -l > test.txt; cat test.txt
```
```bash
cat < test.txt
```
```bash
ls -l >> test.txt
```
```bash
ls -z 2> erreur.txt
```
```bash
ls -z 2>> erreur.txt 
```
```bash
echo "message erreur" >&2
```
```bash
cat fichier_inexistant.txt 2>&1
```
```bash
ls -l |  wc -l
```
```bash
export var "ma variable"
```
```bash
echo $var
```
```bash
unset var
```
```bash
ls -l && echo "ça fonctionne"
```
```bash
ls -z && echo "ça fonctionne"
```
```bash
ls -z || echo "Failed"
```
```bash
ls -l || echo "ça fonctionne"
```
```bash
history
```
```bash
!1
```
```bash
HELP
```

