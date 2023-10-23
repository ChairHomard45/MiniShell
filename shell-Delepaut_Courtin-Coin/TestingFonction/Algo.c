/*
-------------Trim---------------
Fonction trim(chaine)
    Si chaine est nulle
        Renvoyer -1 (erreur)
    Fin Si
    
    longueur = longueur(chaine)
    
    Si longueur == 0
        Renvoyer 0
    Fin Si
    
    start = 0
    end = longueur - 1
    
    Tant que chaine[start] est un espace
        Incrémenter start
    Fin Tant Que
    
    Tant que chaine[end] est un espace
        Décrémenter end
    Fin Tant Que
    
    Si start > end
        Tronquer la chaine à la position start
    Sinon
        Copier les caractères de chaine entre start et end vers le début de la chaine
        Terminer la chaine correctement avec un caractère nul à la position end - start + 1
    Fin Si
    
    Renvoyer longueur(chaine) après la modification

    Fin Fonction
--------------------Clean---------------------
Fonction clean(chaine)
    Si chaine est nulle
        Renvoyer -1 (erreur)
    Fin Si

    longueur = longueur(chaine)
    Si longueur == 0
        Renvoyer 0 (rien à faire)
    Fin Si

    indexEcriture = 0
    indexLecture = 0
    dernierCaractereEspace = faux

    Tant que indexLecture < longueur
        Si chaine[indexLecture] n'est pas un espace, une tabulation ou un saut de ligne
            Si chaine[indexLecture] n'est pas un espace ET dernierCaractereEspace est vrai
                Copier un espace dans chaine[indexEcriture]
                dernierCaractereEspace = vrai
                Incrémenter indexEcriture
            Sinon
                Copier chaine[indexLecture] dans chaine[indexEcriture]
                dernierCaractereEspace = (chaine[indexLecture] est un espace)
                Incrémenter indexEcriture
            Fin Si
        Fin Si
        Incrémenter indexLecture
    Fin Tant Que

    chaine[indexEcriture] = caractère nul // Terminer la chaîne correctement
    Renvoyer indexEcriture
Fin Fonction

--------------Separate_s-----------
fonction separate_s(str, s, max)
    SI str == NULL OU s == NULL OU max <= 0
        RETOURNER -1 // Erreur : paramètres invalides
    FINSI

    taille_str = longueur(str)
    taille_s = longueur(s)
    capacite_actuelle = 0
    index = 0
    nouveau_str = ""

    TANT QUE index < taille_str ET capacite_actuelle < max
        caractere_courant = str[index]

        SI capacite_actuelle + 1 + taille_s <= max
            SI caractere_courant != " " ET position_de(caractere_courant, s) != -1
                // Ajoute un espace avant et après le caractère courant
                nouveau_str += " " + caractere_courant + " "
                capacite_actuelle += 3
            SINON
                // Conserve le caractère sans espace
                nouveau_str += caractere_courant
                capacite_actuelle += 1
            FIN SI
        SINON
            // La capacité maximale est atteinte, arrête le traitement
            SORTIR DE LA BOUCLE
        FIN SI

        index += 1
    FIN TANT QUE

    // Si la capacité n'est pas atteinte, ajoute le reste de la chaîne d'origine
    SI index < taille_str
        nouveau_str += sous-chaîne(str, index, taille_str - index)

    // Copie le résultat dans str
    copier_dans(str, nouveau_str)

    RETOURNER longueur(str)
FIN FONCTION

    */