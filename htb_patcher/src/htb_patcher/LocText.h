#pragma once

namespace htb
{
	constexpr auto MAIN_WINDOW_SELECT = "Selecteer de mappen waarin de bestanden van Spy Fox 1, 2 en 3 zijn. Dit is nodig om de patches correct toe te passen.";
	constexpr auto MAIN_WINDOW_BROWSE = " Bladeren...";
	constexpr auto MAIN_WINDOW_MISSING_FOLDERS_MESSAGE = "Vul alle drie de installatiemappen in voordat je doorgaat.";
	constexpr auto MAIN_WINDOW_PATCH_BUTTON_TEXT = " Patch";
	constexpr auto MAIN_WINDOW_PATCH_BUTTON_CANCEL = " Annuleren";
	constexpr auto MAIN_WINDOW_PATH_TO_SPYFOX_1 = "Path to Spy Fox 1...";
	constexpr auto MAIN_WINDOW_PATH_TO_SPYFOX_2 = "Path to Spy Fox 2...";
	constexpr auto MAIN_WINDOW_PATH_TO_SPYFOX_3 = "Path to Spy Fox 3...";
	constexpr auto MAIN_WINDOW_INSTALL_PATH_SPYFOX_1 = "Spy Fox 1 installatiemap: ";
	constexpr auto MAIN_WINDOW_INSTALL_PATH_SPYFOX_2 = "Spy Fox 2 installatiemap: ";
	constexpr auto MAIN_WINDOW_INSTALL_PATH_SPYFOX_3 = "Spy Fox 3 installatiemap: ";
	constexpr auto MAIN_WINDOW_POPUP_MISSING_FOLDERS = "Patchen mislukt";
	constexpr auto MAIN_WINDOW_POPUP_FAILED = "Patchen mislukt";

	constexpr auto PATCHING_WINDOW_WAITING_TEXT = "De benodigde patches worden nu toegepast op de installatie van SpyFox 3. Dit kan even duren.";

	constexpr auto FINISHED_WINDOW_TEXT = "Klaar is Kees! Jouw Spy Fox 3 is nu in het Nederlands.";
	constexpr auto FINISHED_WINDOW_FOOTER = "Veel speelplezier!";
	constexpr auto FINISHED_WINDOW_SUPPORT = "Support me";

	constexpr auto PATCHER_BUSY_WITH_LOAD_SPY_FOX_3 = "Bezig met het laden van Spy Fox 3...";
	constexpr auto PATCHER_BUSY_WITH_LOADING_BACKGROUND_IMAGES = "Bezig met het laden van achtergrondafbeeldingen...";
	constexpr auto PATCHER_BUSY_WITH_BINDING_SCRIPTS = "Bezig met het koppelen van scripts...";
	constexpr auto PATCHER_BUSY_WITH_BINDING_INDEX = "Bezig met het koppelen van de index info...";
	constexpr auto PATCHER_BUSY_WITH_SEARCHING_TALKIES_SPY_FOX_3 = "Bezig met het zoeken van spraakdata in Spy Fox 3...";
	constexpr auto PATCHER_BUSY_WITH_REPLACING_WITH_SPY_FOX_2_TALKIES = "Bezig met het vervangen van spraak uit Spy Fox 2...";
	constexpr auto PATCHER_BUSY_WITH_REPLACING_WITH_SPY_FOX_1_TALKIES = "Bezig met het vervangen van spraak uit Spy Fox 1...";
	constexpr auto PATCHER_BUSY_WITH_BUILDING_SCRIPTS = "Bezig met het bouwen van de scripts...";
	constexpr auto PATCHER_BUSY_WITH_BUILDING_INDEX = "Bezig met het bouwen van de index...";
	constexpr auto PATCHER_BUSY_WITH_SAVING = "Bezig met het opslaan van bestanden...";

	constexpr auto PATCHER_ERROR_LOAD_SPY_FOX_1_HE4 = "Het laden van HE4 van Spy Fox 1 is mislukt.";
	constexpr auto PATCHER_ERROR_FIND_SONGS_SPY_FOX_1 = "Er zijn geen nummers gevonden in HE4 van Spy Fox 1.";
	constexpr auto PATCHER_ERROR_FIND_SF_SONG_SPY_FOX_1 = "Het Spy Fox-nummer is niet gevonden in Spy Fox 1.";
	constexpr auto PATCHER_ERROR_LOAD_SPY_FOX_1_A = "Het laden van (A) van Spy Fox 1 is mislukt.";
	constexpr auto PATCHER_ERROR_FIND_ROOMS_SPY_FOX_1 = "Er zijn geen rooms gevonden in (A) van Spy Fox 1.";
	constexpr auto PATCHER_ERROR_FIND_LFLF_SPY_FOX_1 = "De LFLF is niet gevonden in Spy Fox 1.";
	constexpr auto PATCHER_ERROR_FIND_END_SOUND_SPY_FOX_1 = "Het eindgeluid is niet gevonden in Spy Fox 1.";

	constexpr auto PATCHER_ERROR_LOAD_ARCHIVES_SPY_FOX_3 = "Het laden van de archieven van Spy Fox 3 is mislukt.";
	constexpr auto PATCHER_ERROR_FIND_IMAGES_SPY_FOX_3 = "Er zijn geen afbeeldingen gevonden in (A) van Spy Fox 3.";
	constexpr auto PATCHER_ERROR_LOAD_IMAGES_SPY_FOX_3 = "Er is iets misgegaan bij het laden van afbeeldingen uit de archieven van Spy Fox 3.";
	constexpr auto PATCHER_ERROR_FIND_HE2_SPY_FOX_3 = "De HE2 is niet gevonden in de archieven van Spy Fox 3.";
	constexpr auto PATCHER_ERROR_FIND_HE0_SPY_FOX_3 = "De HE0 is niet gevonden in de archieven van Spy Fox 3.";
	constexpr auto PATCHER_ERROR_FIND_A_SPY_FOX_3 = "De (A) is niet gevonden in de archieven van Spy Fox 3.";

	constexpr auto PATCHER_ERROR_BIND_SCRIPTS_SPY_FOX_3 = "Het koppelen van de scripts van Spy Fox 3 is mislukt.";
	constexpr auto PATCHER_ERROR_BIND_HE0_SPY_FOX_3 = "Het koppelen van HE0 van Spy Fox 3 is mislukt.";
	constexpr auto PATCHER_ERROR_FIND_TALKS_SPY_FOX_3 = "Er is geen spraakdata gevonden in (A) van Spy Fox 3.";

	constexpr auto PATCHER_ERROR_LOAD_ARCHIVES_SPY_FOX_2 = "Het laden van de archieven van Spy Fox 2 is mislukt.";
	constexpr auto PATCHER_ERROR_FIND_HE2_SPY_FOX_2 = "De HE2 is niet gevonden in de archieven van Spy Fox 2.";
	constexpr auto PATCHER_ERROR_FIND_TALKS_SPY_FOX_2 = "Er is geen spraakdata gevonden in (A) van Spy Fox 2.";

	constexpr auto PATCHER_ERROR_LOAD_ARCHIVES_SPY_FOX_1 = "Het laden van de archieven van Spy Fox 1 is mislukt.";
	constexpr auto PATCHER_ERROR_FIND_HE2_SPY_FOX_1 = "De HE2 is niet gevonden in de archieven van Spy Fox 1.";
	constexpr auto PATCHER_ERROR_FIND_TALKS_SPY_FOX_1 = "Er is geen spraakdata gevonden in (A) van Spy Fox 1.";

	constexpr auto PATCHER_ERROR_BUILD_SCRIPTS_SPY_FOX_3 = "Het opbouwen van de scripts van Spy Fox 3 is mislukt.";
	constexpr auto PATCHER_ERROR_BUILD_HE0_SPY_FOX_3 = "Het opbouwen van HE0 van Spy Fox 3 is mislukt.";
}