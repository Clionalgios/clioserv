Et maintenant ? Encore des incohérences ou des points à éclaircir. Si oui, je veux que tu propose directement la suggestion que tu veux rajouter.

ARBORESCENCE.md :
```/*
├── Header du site
│   ├── Onglets vers sections
│   └── Bouton téléchargement du CV
└── Footer

/
└── Page d'accueil du site web

/whoami/
├── Introduction *Petit paragraphe pour me présenter*
├── Background *Récit biographique d'un paragraphe ou deux de taille moyenne pour expliquer ce qui m'a amené dans la cybersécurité*
├── Projects *Liste des projets réalisés dont pendant les études*
├── Hunting Board *Liste des CTFs et Labs réalisés*
├── Skills *compétences techniques détaillées*
├── Certifications *Téléchargeables pour background checks*
├── CV *PDF téléchargeable*
├── Accounts *Lien vers les comptes HackTheBox, RootMe, Ozintopia, etc*
└── Contact *Informations de contact*

/blog/
├── /blog/browse/{page_number} *Liste des articles avec barre de recherches*
│   ├── Barre de recherche
│   │   ├── Moteur de recherche par opérateurs
│   │   │   ├── Mots
│   │   │   ├── Tags
│   │   │   ├── Langue
│   │   │   └── Date
│   │   └── Cheat sheet avec les opérateurs
│   ├── Activation d'un flux RSS ?? (apprendre ce que c'est pour commencer)
│   ├── Désigner une adresse email pour s'abonner ??
│   ├── Liste de x (selon la pagination définie) articles par page (ou moins si reste inférieur à 10)
│   │   └── *Pour chaque article*
│   │       ├── Image d'illustration
│   │       ├── Titre
│   │       ├── Date de publication
│   │       ├── Auteur
│   │       ├── Tags
│   │       ├── Langues disponibles
│   │       └── Pré-vue du contenu
│   └── Pagination
├── /blog/tag/{tag}/{page_number} *Compilation des articles par tag*
│   ├── *Pour chaque article*
│   │   ├── Image d'illustration
│   │   ├── Titre
│   │   ├── Date de publication
│   │   ├── Auteur
│   │   ├── Tags
│   │   ├── Langues disponibles
│   │   └── Pré-vue du contenu
│   └── Pagination
├── /blog/{slug} *Lecture d'un article*
│   ├── Image de bannière
│   ├── Langues disponibles
│   ├── Titre
│   ├── Date de publication
│   ├── Auteur
│   ├── Tags
│   ├── Contenu
│   └── Articles similaires
└── /blog/rss.xml

/revival/
├── Lien vers le manifeste de l'internet revival
└── The Internet Revival Gateway *Mozaïque de liens-images vers des sites web indépendants, sous forme de tableau*
    └── *Pour chaque objet*
        └── Image-lien

/legal/ *To be defined, and i'm not in a hurry*

/privacy/ *To be defined, and i'm not in a hurry*

/cookies/ *To be defined, and i'm not in a hurry*

/404 *To be defined*

/500 *To be defined*

/sitemap.xml

/robots.txt

*/{lang}/
└── Langue correspondante pour chaque page, à la fin de l'URL

/set-lang/{lang}/

/set-theme/{theme}/

**ANNEXE HISTORIQUE**

Commentaire : l'ensemble de l'arborescence *admin* a été décommissionnée du projet. La documentation ci-dessous est conservée pour rappeler les opérations qui doivent être possibles de réaliser en ligne de commande, qui substituera toutes les opérations d'administration. L'authentification sera intégralement déléguée à un service SSH.
/admin/ *Login si pas authentifié, sinon menu de gestion*
│   ├── /admin/login *redirection vers si pas authentifié*
│   └── Menu de gestion si authentifié
│       └── Onglets
│           ├── Bouton vers monitor
│           ├── Bouton vers l'editoring
│           └── Bouton logout
│               └── Popup de confirmation
│                   └── Activation de la route /admin/logout si confirmé
├── /admin/monitor/ *Informations générales sur l'activité du site*
│   └── TODO
├── /admin/edit/ *Liste des pages dont articles*
│   ├── Table avec la liste des pages du site web
│   └── Boutons "Créer", "éditer" et "supprimer"
│       ├── Si l'utilisateur choisit de créer ou éditer un article, ouvrir un menu d'édition de contenu avec interface d'édition de texte (envisager un framework pour intégrer la fonctionnalité plus facilement)
│       └── Si l'utilisateur choisit de supprimer une page, afficher une pop-up de validation
└── /admin/logout```

CAHIER_DES_CHARGES.md :
```# CAHIER DES CHARGES — Site Web Personnel

---

## 1. Présentation Générale

### 1.1 Contexte

Site web personnel comprenant :
- Un blog technique
- Un book de liens orienté “internet revival”
- Une vitrine professionnelle

Développement :
- En solo
- En C
- HTML et CSS basique
- Sans JavaScript

---

## 2. Objectifs

### 2.1 Objectifs principaux

- Servir de vitrine professionnelle
- Publier des articles (techniques, personnels, pédagogiques, loisirs)
- Centraliser projets et réalisations
- Crédibiliser un positionnement cyber et informatique

## 2.2 Objectifs secondaires

- Expérimentation technique
- Amélioration continue du moteur maison
- SEO minimal
- Mise en place d'un système de swap de thème visuel

---

## 3. Public Cible

- Amis / connaissances
- Recruteurs IT / cybersécurité
- Professionnels techniques
- Communauté CTF / OSINT
- Curieux techniques

---

## 4. Périmètre Fonctionnel (V1)

### 4.0 Global

- Multilingue
- Multithèmes visuels

### 4.1 Pages statiques

- Accueil
- Whoami
- Revival
- Mentions légales
- Politique de confidentialité
- Pages d’erreurs dédiées : 400 / 403 / 404 / 500
- Redirections 301 / 302 basiques

### 4.2 Blog

- Liste paginée
- Barre de recherche (opérateurs inspirés Google)
- Lecture d’article
- Filtrage par tag
- Flux RSS

### 4.3 Téléchargements

- CV (PDF)
- Certifications

---

## 5. Hors Périmètre (Non-objectifs)

- Pas de commentaires
- Pas de multi-auteurs
- Pas d’interface admin web
- Pas de forum
- Pas de monétisation
- Pas d’inscription utilisateur
- Pas d’analytics
- Pas de JavaScript

---

## 6. Architecture et Exigences Techniques

### 6.1 Architecture Générale

- Moteur de templates maison
- Génération dynamique
- Organisation modulaire
- HTTPS obligatoire (HTTP redirigé en 301)
- Dual stack IPv4 + IPv6
- HSTS activé

#### Structure des articles

- Tous les articles dans un répertoire unique
- Un article = un sous-répertoire
- Chaque article contient :
  - Un fichier `main.html` (structure + métadonnées communes + injection template)
  - Un fichier HTML par langue (contenu + images)

#### Statut des articles

Deux statuts uniquement :
- `published`
- `DRAFT`

Règles DRAFT :
- Non listé
- Meta `noindex`
- Exclu sitemap
- Accessible par URL directe
- URL mentionnant expréssement le statut `DRAFT-`

#### Convention URL

Format :

```
/blog/{slug}/{langue}/
```

Règles :
- Slash final = version canonique
- Sans slash → redirection 301
- Sans langue → 404
- Ressource inexistante → 404

Slug :
- Format kebab-case

### Règle de redirection des endpoints de préférence

- `/set-lang/{lang}/` :
    - Définit le cookie `lang`
    - Redirige vers la page précédente si disponible
    - Sinon redirige vers `/blog/browse/page/1/{lang}/`

- `/set-theme/{theme}/` :
    - Définit le cookie `theme`
    - Redirige vers la page précédente

Ces routes ne doivent jamais être indexables.
Elles renvoient systématiquement une redirection 302.

---

## 7. Multilingue

Langues disponibles :

- Français (défaut)
- Anglais
- Espéranto
- Ukrainien
- Allemand

### Règles

- Code ISO 639-1
- Énumération des langues disponibles pour chaque article au lancement serveur
- Version manquante → erreur non-fatale sur console du serveur
- Sélecteur de langue via liens inter-versions
- Affichage des articles correspondant à la langue active (cookie serveur)
- Exception possible via recherche

---

## 8. SEO (Minimal et Structuré)

- `/sitemap.xml`
- `/robots.txt`
- Structure HTML rigoureuse
- Meta tags standards
- OpenGraph
- hreflang
- Canonical strict
- Exclusion sitemap des DRAFT
- Meta `noindex` pour DRAFT
- /sitemap.xml : inclut uniquement articles published
- /robots.txt : interdit les DRAFT-*
- Balises <link rel="canonical"> pour chaque page

---

## 9. Performance

- Pagination obligatoire
- Limitation des requêtes lourdes
- Optimisation images
- Images nommées `{slug}-{index}.png`

---

## 10. Sécurité

### Niveau 1

- Validation stricte des entrées
- Contrôle des requêtes
- Compte système dédié sur VPS
- Permissions minimales
- Politique de logs :
  - Conservation max : 30 jours
  - Suppression automatisée par script
  - Accès root uniquement
  - IP non anonymisées
  - Datage des opérations
  - Contenu de la requête
  - Séparation des informations par un caractère spécial, séparation des différentes entrées par un retour à la ligne
  - Enregistré en fichier texte nommé en fonction du jour du lancement de la session du serveur

### Niveau 2

- Protection XSS
- Protection CSRF
- Rate limiting minimal
- HSTS

---

## 11. Exigences Légales

Cookies :
- Strictement fonctionnels
- Préférences langue / thème / support
- Aucun analytics

RGPD :
- Finalité logs : sécurité et détection d’abus
- Base légale : intérêt légitime
- Durée conservation logs : 30 jours
- Mention explicite dans :
  - Mentions légales
  - Politique de confidentialité

---

## 12. Modèle de Contenu

Encodage :
- UTF-8

Format date :
- ISO 8601

Chaque article contient :

- Header commun (métadonnées + liens langues)
- Métadonnées via balises custom + attributs
- Corps de contenu
- Références images

---

## 13. Convention de Nommage

- **Slugs** : `kebab-case`
- **Tags** : minuscules
- **Langues** : code ISO 639-1 (`fr`, `en`, `eo`, `uk`, `de`)
- **Images** : `{status}{slug}-{index}.png`
- **Statut articles** : `DRAFT-` (draft) / *none* (published)
- **Fichiers articles** : un sous-dossier par article, `main.html` + fichiers par langue (`fr.html`, `en.html`…)

---

## 14. Modèle de Données Logique

### Article

```yaml
Article:
  slug: string
  statut: [draft, published]
  date_publication: ISO8601
  tags: array[string]
  langues_disponibles: array[string]
  chemins_fichiers: array[string]

Langue:
  code: ISO639-1
  nom_affiche: string
  fallback: string (optionnel)
```

---

## 15. Gestion multilingues

- URL canonical : /blog/{slug}/{lang}/
- URL sans slash final → redirection 301 vers canonical
- URL sans langue → 404
- Contenu injecté depuis fichiers lang.html dans main.html
- Affichage selon la langue stockée dans le cookie lang ou paramètre de recherche
- Liens inter-langues : bulles avec drapeaux des équivalents disponibles
- Changement de langue définit par la route `/set-lang/{lang}/` :
    - Définit le cookie `lang`
    - Redirige vers la page précédente
    - Route non-indexable

---

## 16. Gestion multithèmes

- Changement des thèmes définit par un bouton et un menu déroulant.
- Fonctionnement de la gestion multithèmes définie par `/set-theme/{theme}/` :
    - Définit le cookie `theme`
    - Redirige vers la page précédente

---

## 17. Maintenabilité

- Documentation synchronisée
- CLI d’administration
- Sauvegardes via rsync vers serveur secondaire
- Dépendance GitHub pour versionning contenu
- Changelog structuré (secondaire)

---

## 18. Contraintes

- Développement solo
- Budget nul
- VPS minimal
- Maintenance limitée

---

## 19. Cycle de vie du contenu

- Tester au démarrage du serveur, puis à intervalles régulières, que chaque objet dans sitemap ait ses fichiers correspondants et vice-versa, en tenant compte du statut des articles (draft ou published) à partir des critères de statut définis.

---

## 20. Definition of Done

- Toutes les routes fonctionnelles
- Canonical respecté
- Redirections HTTPS effectives
- Swap de langue opérationnel
- Swap de thème fonctionnel
- RSS valide
- Sitemap valide
- Pas d’erreurs serveur en navigation normale
- Documentation synchronisée
- Effacement automatique des logs au bout de 30 jours (crontab quotidien)

---

## 21. État de Complétude

| Domaine | Couverture |
|----------|------------|
| Vision stratégique | ✔ |
| Périmètre fonctionnel | ✔ |
| Non-objectifs | ✔ |
| Architecture | ✔ |
| Sécurité | Partiel |
| SEO | Basique |
| Modèle de données | Défini |
| Cycle de vie contenu | Non formalisé |
| Médias | Minimal |
| Logging détaillé | Partiel |
| Gouvernance technique | Partiel |

---```