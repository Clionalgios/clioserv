# 1. Contexte

Site web perso avec blog technique et un book de liens en soutien de l'internet revival
Développement en solo en C, html et css basique

# 2. Objectifs

## 2.1 Objectifs principaux

Me servir de vitrine professionnelle
Publier des articles (techniques, personnels, pédagogiques, loisirs)
Centraliser mes projets et réalisations
Crédibiliser mon positionnement cyber et informatique

## 2.2 Objectifs secondaires

Expérimentation technique
Amélioration continue du moteur maison
SEO minimal

## 2.3 Stade actuel de complétion  du cahier des charges par ChatGPT :

| Domaine | Couverture |
| --- | --- |
| Vision stratégique |	✔ |
| Périmètre fonctionnel	| ✔ |
| Non-objectifs	| ✔ |
| Architecture générale	| ✔ |
| Sécurité	| Partiellement complet |
| SEO	| Basique |
| Modèle de données	| ❌ |
| Cycle de vie contenu	| ❌ |
| Médias	| ❌ |
| Logging détaillé	| ❌ |
| Gouvernance technique	| Partiel |

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
Erreurs 404 / 500 / 400 / 403 -> pages d'erreurs dédiées
301 / 302 -> Redirections basiques

## 4.2 Blog

Liste paginée (paramétrée par défaut avec recherche sans critères)
Barre de recherche avec Opérateurs dérivés des opérateurs de recherche Google
Lecture d’article
Filtrage par tag
Multilingue
RSS

## 4.3 Téléchargements

CV PDF
Certifications

# 5. Périmètre hors-scope (Non-objectifs)

Pas de commentaires natifs
Pas de système multi-auteurs
Pas d’interface admin web
Pas de forum
Pas de monétisation
Pas d’inscription utilisateur
Pas de cookie analytics

# 6. Exigences techniques

## 6.1 Architecture

Moteur de templates maison
Génération dynamique
Organisation modulaire
Full HTTPS, no HTTP (redirection 301 vers https)
Dual IPv6 et IPv4
Articles regroupés dans un seul et même répertoire. Un article = un sous-répertoire. Dans un sous-répertoire d'article = un fichier html main, incluant une balise qui contiendra les métadonnées communes à toutes les versions et une balise d'injection de template, un fichier html par langue, contenant le contenu textuel de l'article et les images ensuite injecté dans le fichier html main de l'article. Statut des articles géré par mot-clé (si DRAFT-{slug}, pas référencé dans l'énumération des articles sur le blog ni indexé ou indexable (meta noindex obligatoire). Reste accessible par URL à tous les utilisateurs). Deux statuts seulement : publié et draft.
Slug en kebab-case
JavaScript minimal (pour l'utilisation des cookies d'informations de session (langue/média/thème visuel))


## 6.2 Multilingue

Stratégie URL *Exemple : /blog/analyse-xss/fr pour un article en version française ou /blog/analyse-xss/ukr pour un article en version ukrainienne*. Toute ressource inexistante renvoie 404. Slash final optionnel
Gestion des traductions avec énumération des articles et de la disponibilité de chaque langue au lancement du serveur, avec renvoi d'erreur non-fatale si certaines versions linguistiques sont manquantes.
Lien entre versions, avec liens sous forme de "bulle" avec drapeaux des langues en question disponibles inclues dans la "feuille de l'article" (dont le contenu réel est injecté depuis le fichier texte du contenu d'article en question).
Afficher tous et seulement les articles de la langue en cours d'usage (cookies), sauf si indication contraire en barre de recherche

## 6.3 SEO minimal

/sitemap.xml (déréférencement de tous les DRAFT-*)
/robots.txt (déréférencement de tous les DRAFT-*)
Structure html rigoureuse
Meta tags
OpenGraph
hreflang
Stratégie d’indexation multilingue (utilisation des balises canonical)

## 6.4 Performance

Pagination
Limitation requêtes lourdes
Optimisation images

# 7. Exigences de sécurité

Niveau 1 :
Validation stricte des entrées
Contrôle des requêtes
Politique de logs (durée de conservation : 10 itérations du serveur, accès restreint à admin (root), pas d'anonymisation IP)
Création de compte utilisateur sur le VPS spécifique à l'utilisation du serveur web, avec permissions dédiées.

Niveau 2 :
Protection XSS
Protection CSRF
Rate limiting minimal
HSTS

# 8. Exigences légales

Cookies (si utilisés), notamment de langue, de préférence de thème visuel et de support d'utilisation, cookie spécifique pour l'accord des cookies autorisés. Cookies strictement fonctionnel, 0 analytics

Effacement des logs au bout de 30 jours (géré par script) pour respecter les RGPD. Mentionner dans les mentions légales et la politique de confidentialité
Finalité : sécurité et détection d’abus
Durée exacte (ex : 30 jours)
Base légale : intérêt légitime

Conformité RGPD

Mentions légales
Politique de confidentialité

# 9. Exigences de maintenabilité

Documentation à jour
CLI d’administration
Utilisation de rsync pour la gestion des sauvegardes


Secondaire :
Changelog structuré

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
Fonctionnement en https://, non-fonctionnement en http://

# Section 12 — Modèle de contenu

Les articles sont stockés sous forme de fichiers html.
Chaque article contient :
- Cadre oecuménique, incluant un header avec liens vers les versions linguistiques disponibles et métadonnées (titre, date, tags, langue, slug, statut)
- Corps de contenu (incluant les informations métadonnées dans la langue disponible)
- Références images

# Section 13 — Convention de nommage

Slugs : kebab-case
Tags : minuscules
Langues : code ISO 639-1
Images : {slug}-{index}.webp

# Section 14 — Langues disponibles

- Français (langue par défaut)
- Anglais
- Espéranto
- Ukrainien
- Allemand