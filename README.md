# MiniShell

MiniShell doit être capable de réaliser d'utiliser au minium:
- Les commandes intégrés comme $cd [dir] ou $exit [n].
- Des commandes simples en 'premier plan' ou 'arriere plan'  $commande ou $commande & ou $commande1 ; commande2 ; commande3 & commande4 ; ... .
- Les redirections entre-sortie:
    $commande > file.output
    $commande 2> file.error
    $commande < file.input
    $commande >> file.appout
    $commande 2>> file.apperr
    $commande >&2
    $commande 2>&1
    $commande1 | commande2 | commande3 | ..
- De gérer les variables d’environnement:
    $export VAR="ma variable"
    $echo $VAR
    $unset VAR
- De gérer les opérateurs !, && et || comme les gère le bash.
