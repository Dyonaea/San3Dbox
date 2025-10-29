# San3Dbox

Le but du projet est de créer un monde "sandbox" mais en 3D, pour pouvoir enrichir mes compétences en 3D
et en programmation bas niveau tout en créant un univers amusant.

## Déploiement

### Prérequis

- CMake ≥ 3.10  
- Compilateur C++17 ou supérieur (GCC, Clang ou MSVC)  
- Git (pour cloner le dépôt)

### Étapes d’installation et de compilation

Cloner le projet
```bash
git clone https://github.com/<votre-utilisateur>/San3Dbox.git
cd San3Dbox
mkdir build
cd build
cmake ..
cmake --build .
```
## Structure du projet
```
San3Dbox/
├── src/
│   ├── client/        # Code du client (réseau, rendu OpenGL, interactions)
│   ├── server/        # Code du serveur (monde, synchronisation, logique)
│   ├── world/         # Comportement et données du monde partagé
│   └── util/          # Fonctions utilitaires communes
│
├── include/
│   ├── San3Dbox/      
│   └── ...
│
├── CMakeLists.txt     # Configuration principale du projet
└── README.md          # Ce fichier
```