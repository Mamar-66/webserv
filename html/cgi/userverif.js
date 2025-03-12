#!/usr/bin/env node

const fs = require('fs');
const path = require('path');
const url = require('url');

// Fonction pour générer la réponse HTTP
function responseHttp(statusCode, userID, userName) {
	let htmlContent = '';
	const statusMessages = {
		200: 'OK',
		500: 'Internal Server Error',
		401: 'Internal Server Error',
	};

	let redirectUrl = '';
	if (statusCode === 200)
		redirectUrl = '../conect/index.html';
	else if (statusCode === 500)
		redirectUrl = '../conect/errors/errors_500/error_500.html';
	else if (statusCode === 401)
		redirectUrl = '../error_conect.html';

	const reason = statusMessages[statusCode] || 'Unknown Status';

	let response = `HTTP/1.1 ${statusCode} ${reason}\r\n`;

	try {
		htmlContent = fs.readFileSync(path.join(__dirname, redirectUrl), 'utf-8');
	} catch (err) {
		htmlContent = '';
	}

	response += `Content-Type: text/html\r\n`;
	// response += `Content-Length: ${Buffer.byteLength(htmlContent)}\r\n`;
	response += `Connection: close\r\n`;
	response += `Set-Cookie: ${userName}|${userID}\r\n\r\n`;

	console.log(response);
	console.log(htmlContent);
}

// Fonction pour analyser les données du formulaire (simule une requête POST ou GET)
function getData() {
	const method = process.env.METHOD;

	let params = [];
	if (method === 'POST') {
		let data = ''; // Chaîne pour accumuler les données

		// Écoute de l'événement 'data' sur stdin pour récupérer les morceaux de données
		process.stdin.on('data', chunk => {
			data += chunk;
		});

		// Lorsque stdin est complètement lu, on traite les données
		process.stdin.on('end', () => {
			try {
				// Conversion des données en paramètres
				params = new url.URLSearchParams(data);
				// Appel de la fonction qui traite les paramètres
				handleRequest(params);
			} catch (error) {
				process.stderr.write("Erreur lors de l'analyse des données\n");
				responseHttp(500, '', '');
			}
		});

		// Écoute en cas d'erreur de lecture du flux
		process.stdin.on('error', (err) => {
			process.stderr.write("Erreur lors de la lecture du flux stdin\n");
			responseHttp(500, '', '');
		});

	} else if (method === 'GET') {
		const query = process.env.FORM;
		// Traitement direct de la query (s'il s'agit d'une requête GET)
		params = new url.URLSearchParams(query);
		handleRequest(params);
	}
}

// Fonction pour gérer la demande après avoir récupéré les données
function handleRequest(params) {
	const user = (params.get('user') || '').trim();
	const password = (params.get('password') || '').trim();

	if (user === '' || password === '') {
		process.stderr.write("erreur lectur\n");
		responseHttp(500, '', '');
		return;
	}

	checkLog(user, password);
}

// Fonction pour vérifier les informations de l'utilisateur
function checkLog(user, password) {
	const filePath = path.join(__dirname, '../../json/users.json');

	fs.readFile(filePath, 'utf-8', (err, data) => {
		if (err) {
			process.stderr.write("Erreur de lecture du fichier JSON\n");
			responseHttp(500, '', '');
			return;
		}

		process.stderr.write("Contenu brut du fichier JSON :\n");
		process.stderr.write(data + "\n");

		// Vérifier si le contenu est une chaîne non vide
		if (typeof data !== 'string' || !data.trim()) {
			process.stderr.write("Le fichier JSON est vide ou invalide\n");
			responseHttp(500, '', '');
			return;
		}

		// Essai de parsing du JSON
		process.stderr.write("Essai de parsing de la chaîne JSON\n");
		let usersData;
		try {
			usersData = JSON.parse(data.trim());
		} catch (parseError) {
			process.stderr.write("Erreur de parsing du JSON : " + parseError.message + "\n");
			responseHttp(500, '', '');
			return;
		}

		// Vérifier si c'est bien un tableau
		if (!Array.isArray(usersData)) {
			process.stderr.write("Le JSON ne contient pas un tableau valide !\n");
			responseHttp(500, '', '');
			return;
		}

		process.stderr.write("Données JSON analysées :\n");
		process.stderr.write(JSON.stringify(usersData, null, 2) + "\n");

		// Vérification des identifiants
		process.stderr.write(`User recherché : ${user}\n`);
		process.stderr.write(`Mot de passe recherché : ${password}\n`);

		const matchingUser = usersData.find(u => u.name === user && u.password === password);

		if (matchingUser) {
			process.stderr.write("Utilisateur trouvé : " + matchingUser.name + matchingUser.userID + "\n");
			responseHttp(200, matchingUser.userID, matchingUser.name);
		} else {
			process.stderr.write("Aucun utilisateur trouvé avec les identifiants donnés\n");
			responseHttp(401, '', '');

		}
	});
}

// Appel de la fonction principale
getData();
