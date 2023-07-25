# ft_irc

ft_irc est un serveur IRC développé en C++ 98 dans le cadre du projet de l'école 42. Le serveur est conçu pour gérer plusieurs clients simultanément sans jamais bloquer.

## Client de référence

Le client de référence utilisé pour ce projet est LimeChat.

## Utilisation

Pour installer le projet, clonez le dépôt et utilisez la commande `make` pour compiler le projet. Ensuite, exécutez le programme `ircserv` avec le numéro du port en premier paramètre et le mot de passe du serveur en deuxième paramètre.

Exemple d'utilisation :

`./ircserv <port> <password>`

## Fonctionnalités principales

- Authentification des clients
- Rejoindre un channel
- Envoi et réception de messages privés
- Transmission des messages envoyés dans un channel à tous les clients qui l'ont rejoint
- Gestion des opérateurs et des utilisateurs basiques

## Commandes disponibles

Le serveur ft_irc implémente les commandes suivantes :

- `INVITE` : Permet d'inviter un utilisateur à rejoindre un channel spécifique.
- `JOIN` : Permet à un utilisateur de rejoindre un channel existant.
- `KICK` : Permet de kicker un utilisateur d'un channel donné.
- `MODE` : Permet de modifier les modes d'un utilisateur ou d'un channel.
  - `Flag k` : Active/Désactive le mot de passe d'un channel
  - `Flag i` : Active/Désactive le mode sur invitation seulement d'un channel
  - `Flag t` : Active/Désactive les restriction pour modifier le topic d'un channel par un utilisateur
  - `Flag o`: Donne/retire les droits d'operateur du channel
  - `Flag l`: Active/Désactive une limite d'utilisateur dans un channel
- `NICK` : Permet à un utilisateur de changer de pseudo (nickname).
- `NOTICE` : Permet d'envoyer un message privé à un autre utilisateur.
- `OPER` : Permet d'accorder les privilèges d'opérateur à un utilisateur.
- `PART` : Permet à un utilisateur de quitter un channel.
- `PASS` : Permet à un client de fournir le mot de passe pour s'identifier auprès du serveur.
- `PRIVMSG` : Permet d'envoyer un message à un channel ou à un utilisateur spécifique.


## Outils supplémentaires

Pour interagir avec le serveur ft_irc, vous pouvez utiliser les outils supplémentaires suivants :

- **Netcat** : Utilisé pour établir une connexion TCP/IP avec le serveur et envoyer des commandes ou des messages.
- **LimeChat** : Utilisé comme client de référence pour se connecter à votre serveur ft_irc. Vous pouvez utiliser LimeChat pour effectuer les fonctionnalités de base requises.

## Collaborateurs

- [Mbertin44](https://github.com/Mbertin44)
- [OSS-42](https://github.com/OSS-42)
- [Anasilvr](https://github.com/anasilvr)

N'hésitez pas à nous contacter si vous avez des questions ou des commentaires sur ce projet.

## Bot ChuckNorris (Bonus)

Nous avons également ajouté un bot ChuckNorris en PHP qui génère aléatoirement des anecdotes sur Chuck Norris.

Pour l'utiliser il faut :

- Lancer le serveur
- Ce rendre dans le dossier bonus puis executer la commande : php ChuckNorris.php
- Connecter un utilisateur et lui faire rejoindre le channel #test
- Utiliser la commande !ChuckNorris dans le channel avec le nouvel utilisateur connecté au channel.
- Dans le cas ou le client est NetCat il faut exécuter la commande : PRIVMSG #test !ChuckNorris
