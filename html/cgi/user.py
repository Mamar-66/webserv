#!/usr/bin/python3

import os
import sys
import time


data = sys.stdin.read()
sys.stdout = os.fdopen(sys.stdout.fileno(), 'w', buffering=1)


html_content = """<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Connexion Reussie</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #e7f7e7;
            text-align: center;
            padding: 50px;
        }
        h1 {
            color: #4CAF50;
        }
        p {
            color: #333;
        }
        .success-box {
            background-color: #d4edda;
            color: #155724;
            padding: 20px;
            border: 1px solid #c3e6cb;
            display: inline-block;
            margin-top: 20px;
        }
    </style>
</head>
<body>
    <h1>Connexion reussie</h1>
    <div class="success-box">
        <p><strong>Felicitations !</strong> Vous etes maintenant connecte.</p>
        <p>Merci de vous etre connecte avec succes. Vous pouvez maintenant acceder a votre compte.</p>
    </div>
</body>
</html>"""

# Construction de la reponse HTTP avec en-tetes pour code 200
reponse = "HTTP/1.1 200 OK\r\n"  # Code 200 pour OK
reponse += "Content-Type: text/html; charset=UTF-8\r\n" # Type du contenu
# reponse += "Content-Length: {}\r\n".format(len(html_content)) # Longueur du contenu
reponse += "Connection: close\r\n"# Connexion fermee après la reponse
reponse += "\r\n"# Ligne vide pour separer l'en-tete du corps

# print(f"reponce : {reponse}\nlen : {len(reponse) + len(html_content)}", file=sys.stderr)
# time.sleep(2)
  # Corps de la reponse contenant du HTML

# Afficher la reponse complète (en-tete + corps)
# print(reponse + html_content, end="")
print(reponse, end="")

print('<!DOCTYPE html>');
print('<html lang="fr">');

print('<head>');
print('    <meta charset="UTF-8">');
time.sleep(1)
print('    <meta name="viewport" content="width=device-width, initial-scale=1.0">');
print('    <title>Connexion Reussie</title>');
print('    <style>');
print('        body {');
print('            font-family: Arial, sans-serif;');
time.sleep(1)
print('            background-color: #e7f7e7;');
print('            text-align: center;');
print('            padding: 50px;');
print('        }');
time.sleep(1)
print('        h1 {');
print('            color: #4CAF50;');
print('        }');
print('        p {');
print('            color: #333;');
print('        }');
print('        .success-box {');
print('            background-color: #d4edda;');
print('            color: #155724;');
time.sleep(1)
print('            padding: 20px;');
print('            border: 1px solid #c3e6cb;');
print('            display: inline-block;');
print('            margin-top: 20px;');
print('        }');
print('    </style>');
print('</head>');
print('<body>');
print('    <h1>Connexion reussie</h1>');
print('    <div class="success-box">');
print('        <p><strong>Felicitations !</strong> Vous etes maintenant connecte.</p>');
print('        <p>Merci de vous etre connecte avec succes. Vous pouvez maintenant acceder a votre compte.</p>');
print('    </div>');
print('</body>');
print('</html>', end="");
time.sleep(1)

# fd,fd2 = os.getenv("FD").split("/")
# print("fiiiinnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn", file=sys.stderr)
# print("eeeeeeeee", fd, file=sys.stderr)
# if fd is not None:
#     try:
#         # Convertir la valeur en entier et fermer le descripteur de fichier
#         os.close(int(fd))
#         print(f"Descripteur de fichier {fd} fermé avec succès.", file=sys.stderr)
#     except ValueError:
#         print(f"Erreur : la valeur de 'FD' ({fd}) n'est pas un nombre valide.", file=sys.stderr)
#     except OSError as e:
#         print(f"Erreur : impossible de fermer le descripteur de fichier {fd}. Détails : {e}", file=sys.stderr)
# else:
#     print("Erreur : la variable d'environnement 'FD' n'est pas définie.", file=sys.stderr)

# if fd2 is not None:
#     try:
#         # Convertir la valeur en entier et fermer le descripteur de fichier
#         os.close(int(fd2))
#         print(f"Descripteur de fichier {fd2} fermé avec succès.", file=sys.stderr)
#     except ValueError:
#         print(f"Erreur : la valeur de 'FD' ({fd2}) n'est pas un nombre valide.", file=sys.stderr)
#     except OSError as e:
#         print(f"Erreur : impossible de fermer le descripteur de fichier {fd2}. Détails : {e}", file=sys.stderr)
# else:
#     print("Erreur : la variable d'environnement 'FD' n'est pas définie.", file=sys.stderr)