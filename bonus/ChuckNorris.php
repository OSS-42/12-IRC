// --------  USAGE  ----------
//Lance le serveur, plusieurs clients, puis dans un terminal tu fais :
//cd bonus
//php ChuckNorris.php

//Il joint automatiquement un channel appelé #test.

//Fait joindre les autres clients à ce channel.
//Puis a l'un d'entre eux tu tapes "!ChuckNorris"
//et il devrait se passer quelquechose.
//------------------------------


<?php

//parameters
$server = "127.0.0.1";
$port = "6667";
$password = "momo";
$channel = "#test";

// Connect to the server
$socket = stream_socket_client("tcp://$server:$port", $errno, $errstr);


if (!$socket) {
	echo "BOT >>> Unexpected error: $errstr ($errno) <<< \n";
	exit(1);
}

// Log on to the server
fwrite($socket, "PASS $password\r\n");
sleep(1);
fwrite($socket, "NICK Chuck\r\n");
sleep(1);
fwrite($socket, "USER Norris 0 * :Chuck Norris Bot\r\n");
sleep(1);

// Join a channel
fwrite($socket, "JOIN $channel\r\n");

// Process incoming messages
while ($data = fgets($socket)) {
	// Split the data into parts
	$parts = explode(' ', $data);

	// Check for a PING message and respond with PONG
	if ($parts[0] == 'PING') {
		fwrite($socket, 'PONG ' . $parts[1] . "\r\n");
	}

	// Check for PRIVMSG command
	if (count($parts) >= 4 && $parts[1] == 'PRIVMSG') {
		// Check if the message contains the '!ChuckNorris' command
		if (strpos($parts[3], '!ChuckNorris') !== false) {
			$quotes = array(
				"Chuck Norris a retrouvé Ornicar.",
				"Chuck Norris fait pleurer les oignons.",
				"Jesus Christ est né en 1940 avant Chuck Norris.",
				"Chuck Norris est la raison pour laquelle Charlie se cache.",
				"Chuck Norris a déjà compté jusqu'à l'infini. Deux fois.",
				"Tous les soirs, les cauchemars font le même Chuck Norris.",
				"Chuck Norris est capable de laisser un message avant le bip sonore.",
				"Chuck Norris a gagné un tournoi de poker avec des cartes Pokémon.",
				"Chuck Norris ne porte pas de montre. Il décide de l'heure qu'il est.",
				"Quand Google ne trouve pas quelque chose, il demande à Chuck Norris.",
				"Chuck Norris et Superman ont fait bras de fer, le perdant devait mettre son slip par dessus son pantalon.",
				"Quand la tartine de Chuck Norris tombe, la confiture change de côté.",
				"Un jour, Chuck Norris a plongé dans la Mer Rouge. Moïse en a profité pour passer."
			);

			$choice = rand(0, count($quotes) - 1);
			$quote = $quotes[$choice];

			// Respond with the quote (the target is the second part of the PRIVMSG command)
			// fwrite($socket, 'PRIVMSG ' . $parts[2] . ' :' . $quote . "\r\n");
			fwrite($socket, 'PRIVMSG ' . $channel . ' :' . $quote . "\r\n");
		}
	}
}

fclose($socket);

?>