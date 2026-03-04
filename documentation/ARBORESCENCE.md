/*
├── Header du site
│   ├── Onglets vers sections
│   └── Bouton téléchargement du CV ?
├── Bouton musique ?
└── Footer ?

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
├── /blog/browse/page/{page_number} *Liste des articles avec barre de recherches*
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
├── /blog/tag/{tag} *Compilation des articles par tag*
│   ├── /blog/tag/{tag}/page/{page_number} *Compilation des articles par tag*
│   │   └── Liste de x (selon la pagination définie) articles par page (ou moins si reste inférieur à 10)
│   │       └── *Pour chaque article*
│   │           ├── Image d'illustration
│   │           ├── Titre
│   │           ├── Date de publication
│   │           ├── Auteur
│   │           ├── Tags
│   │           ├── Langues disponibles
│   │           └── Pré-vue du contenu
│   └── Pagination
├── /blog/read/{slug} *Lecture d'un article*
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
└── /admin/logout

/legal/ *To be defined, and i'm not in a hurry*

/privacy/ *To be defined, and i'm not in a hurry*

/cookies/ *To be defined, and i'm not in a hurry*

/404 *To be defined*

/500 *To be defined*

/sitemap.xml

/robots.txt

*/{lang}/
└── Langue correspondante pour chaque page