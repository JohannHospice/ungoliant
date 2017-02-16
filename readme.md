# Ungoliant: Robot d’Indexation et Moteur de Recherche
Le but de ce projet est d’implémenter en langage C un moteur de recherche, un programme capable d’explorer récursivement le contenu d’un ensemble de pages HTML ou de sites Web, de les indexer, et de permettre à un utilisateur d’effectuer une recherche sur leur contenu.

Le projet comprend quatre parties principales. Il vous faudra implémenter :
1. un analyseur syntaxique (parser) de pages HTML ;
2. un gestionnaire de fichier d’index, ce dernier contenant les informations nécessaires pour associer un ensemble de pages à un mot-clef donné, le gestionnaire permettant d’effectuer, ajouter, supprimer ou mettre à jour de telles associations ;
3. un robot d’indexation (crawler), ou araignée, qui télécharge chaque page, analyse leur contenu à l’aide du parser, ajoute de l’information à l’index et se déplaçe de page en page en suivant les liens externes ;
4. enfin, un gestionnaire de requêtes (query engine) permettant à un utilisateur de retrouver des pages indexées à partir d’un ou plusieurs mots-clefs. 

Le projet est à faire en trinômes. La progression régulière de votre travail au cours du semestre, de même que le bon usage des outils et des pratiques de conduite de projet présentés dans ce cours, seront au moins aussi importantes pour votre évaluation que le résultat final. Les informations pratiques utiles vous seront fournies sur [la page Moodle du cours](http://moodlesupd.script.univ-paris-diderot.fr/course/view.php?id=6660).

## Instructions
initialisation de l'espace de travail:
```bash
make init
```
### Compilation
```bash
make ungoliant
```
#### Test
```bash
make [module]-test
```
### Execution
Les executables se trouvent dans le dossier `prod/`
