# 1. Contexte

Site web perso avec blog technique et un book de liens en soutien de l'internet revival
Développement en solo en C, html et css basique

# 2. Objectifs

## 2.1 Objectifs principaux

Présenter ton profil
Publier des articles techniques
Centraliser tes projets et réalisations
Crédibiliser ton positionnement cyber et IT

## 2.2 Objectifs secondaires

Expérimentation technique
Amélioration continue du moteur maison
SEO minimal

# 3. Public cible

Amis / connaissances
Recruteurs IT / cybersécurité
Professionnels techniques
Communauté CTF / OSINT
Curieux techniques

# 4. Périmètre fonctionnel (V1)

## 4.1 Pages statiques

Accueil
Whoami
Revival
Pages légales
Erreurs 404 / 500

## 4.2 Blog

Liste paginée (paramétrée par défaut avec recherche sans critères)
Barre de recherche avec Opérateurs dérivés des opérateurs de recherche Google
Lecture d’article
Filtrage par tag
Multilingue
RSS

## 4.4 Téléchargements

CV PDF
Certifications

# 5. Périmètre hors-scope (Non-objectifs)

Pas de commentaires natifs
Pas de système multi-auteurs
Pas d’interface admin web
Pas de forum
Pas de monétisation
Pas d’inscription utilisateur

# 6. Exigences techniques

## 6.1 Architecture

Moteur de templates maison
Génération dynamique
Organisation modulaire

## 6.2 Multilingue

Stratégie URL *Exemple : /blog/analyse-xss/fr pour un article en version française ou /blog/analyse-xss/ukr pour un article en version ukrainienne*
Gestion des traductions avec énumération des articles et de la disponibilité de chaque langue au lancement du serveur, avec renvoi d'erreur non-fatale si certaines versions linguistiques sont manquantes.
Lien entre versions, avec liens sous forme de "bulle" avec drapeaux des langues en question disponibles inclues dans la "feuille de l'article" (dont le contenu réel est injecté depuis le fichier texte du contenu d'article en question).
Afficher tous et seulement les articles de la langue en cours d'usage (cookies), sauf si indication contraire en barre de recherche

## 6.3 SEO minimal

Sitemap
Robots.txt
Meta tags
OpenGraph

## 6.4 Performance

Pagination
Limitation requêtes lourdes
Optimisation images

# 7. Exigences de sécurité

Niveau 1 :
Validation stricte des entrées
Contrôle des requêtes
Politique de logs

Niveau 2 :
Protection XSS
Protection CSRF
Rate limiting minimal
Gestion sécurisée des uploads

# 8. Exigences légales

Mentions légales
Politique de confidentialité
Cookies (si utilisés), notamment de langue, de préférence de thème visuel
Conformité RGPD

# 9. Exigences de maintenabilité

Documentation à jour
CLI d’administration
Changelog structuré
Stratégie de sauvegarde

# 10. Contraintes

Développement seul
Budget nul
Hébergement simple (serveur VPS minimal)
Maintenance limitée en temps

# 11. Critères de validation (Definition of Done)

Toutes les routes définies fonctionnent
Swap de langue en cours d'utilisation ou lecture article (afficher les langues où l'article est dispo pendant la lecture)
RSS valide
Sitemap valide
Pas d’erreur serveur sur navigation normale
Documentation synchronisée