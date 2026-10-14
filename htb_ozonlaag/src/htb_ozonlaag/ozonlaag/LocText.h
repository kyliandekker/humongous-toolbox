#pragma once

namespace htb::ozonlaag
{
	// WINDOW TEXT
	constexpr auto MAIN_WINDOW_SELECT = "Selecteer de mappen waarin de bestanden van Spy Fox 1, 2 en 3 zijn. Dit is nodig om de patches correct toe te passen.";
	constexpr auto MAIN_WINDOW_WARNING = "Zorg ervoor dat Spy Fox 1 en 2 de Nederlandse installaties zijn.";
	constexpr auto MAIN_WINDOW_BROWSE = " Bladeren...";
	constexpr auto MAIN_WINDOW_MISSING_FOLDERS_MESSAGE = "Vul alle drie de installatiemappen in voordat je doorgaat.";
	constexpr auto MAIN_WINDOW_PATCH_BUTTON_TEXT = " Patch";
	constexpr auto MAIN_WINDOW_PATCH_BUTTON_CANCEL = " Annuleren";
	constexpr auto MAIN_WINDOW_PATH_TO_SPYFOX_1 = "Pad naar Spy Fox 1...";
	constexpr auto MAIN_WINDOW_PATH_TO_SPYFOX_2 = "Pad naar Spy Fox 2...";
	constexpr auto MAIN_WINDOW_PATH_TO_SPYFOX_3 = "Pad naar Spy Fox 3...";
	constexpr auto MAIN_WINDOW_INSTALL_PATH_SPYFOX_1 = "Spy Fox 1 installatiemap: ";
	constexpr auto MAIN_WINDOW_INSTALL_PATH_SPYFOX_2 = "Spy Fox 2 installatiemap: ";
	constexpr auto MAIN_WINDOW_INSTALL_PATH_SPYFOX_3 = "Spy Fox 3 installatiemap: ";
	constexpr auto MAIN_WINDOW_POPUP_MISSING_FOLDERS = "Patchen mislukt";
	constexpr auto MAIN_WINDOW_POPUP_FAILED = "Patchen mislukt";
	constexpr auto MAIN_WINDOW_FIND_INSTALLATION = "Vind installaties";

	constexpr auto PATCHING_WINDOW_WAITING_TEXT = "De benodigde patches worden nu toegepast op de installatie van Spy Fox 3. Dit kan even duren.";

	constexpr auto FINISHED_WINDOW_TEXT = "Klaar is Kees! Jouw Spy Fox 3 is nu in het Nederlands.";
	constexpr auto FINISHED_WINDOW_FOOTER = "Veel speelplezier!";
	constexpr auto FINISHED_WINDOW_SUPPORT = "Support me";

	// PATCHER STATUS
	constexpr auto PATCHER_BUSY_WITH_LOAD_SPY_FOX_1_MUSIC = "Bezig met het laden van Spy Fox 1 muziek.";
	constexpr auto PATCHER_BUSY_WITH_LOAD_SPY_FOX_1_SOUNDS = "Bezig met het laden van Spy Fox 1 sounds.";
	constexpr auto PATCHER_BUSY_WITH_LOAD_SPY_FOX_3 = "Bezig met het laden van Spy Fox 3.";
	constexpr auto PATCHER_BUSY_WITH_LOADING_BACKGROUND_IMAGES = "Bezig met het laden van achtergrondafbeeldingen.";
	constexpr auto PATCHER_BUSY_WITH_BINDING_SCRIPTS = "Bezig met het koppelen van scripts.";
	constexpr auto PATCHER_BUSY_WITH_BINDING_INDEX = "Bezig met het koppelen van de index info.";
	constexpr auto PATCHER_BUSY_WITH_SEARCHING_TALKIES_SPY_FOX_3 = "Bezig met het zoeken van spraakdata in Spy Fox 3.";
	constexpr auto PATCHER_BUSY_WITH_SEARCHING_IMAGES_SPY_FOX_3 = "Bezig met het zoeken van plaatjes in Spy Fox 3.";
	constexpr auto PATCHER_BUSY_WITH_REPLACING_WITH_SPY_FOX_2_TALKIES = "Bezig met spraakdata van Spy Fox 3 vervangen met die van Spy Fox 2.";
	constexpr auto PATCHER_BUSY_WITH_REPLACING_WITH_SPY_FOX_1_TALKIES = "Bezig met spraakdata van Spy Fox 1 vervangen met die van Spy Fox 2.";
	constexpr auto PATCHER_BUSY_WITH_BUILDING_SCRIPTS = "Bezig met het bouwen van de scripts.";
	constexpr auto PATCHER_BUSY_WITH_BUILDING_INDEX = "Bezig met het bouwen van de index.";
	constexpr auto PATCHER_BUSY_WITH_REPLACING_IMAGES_FROM_SPY_FOX_2 = "Bezig met plaatjes van Spy Fox 3 vervangen met die van Spy Fox 2.";
	constexpr auto PATCHER_BUSY_WITH_REPLACING_BACKGROUND_FROM_SPY_FOX_2 = "Bezig met achtergronden van Spy Fox 3 vervangen met die van Spy Fox 2.";
	constexpr auto PATCHER_BUSY_WITH_SAVING = "Bezig met het opslaan van bestanden.";

	// PATCHER ERRORS
	constexpr auto PATCHER_ERROR_LOAD_SPY_FOX_1_HE4 = "Het laden van de HE4 van Spy Fox 1 is mislukt.";
	constexpr auto PATCHER_ERROR_LOAD_SPY_FOX_1_A = "Het laden van de (A) van Spy Fox 1 is mislukt.";
	constexpr auto PATCHER_ERROR_LOAD_SPY_FOX_1_HE2 = "Het laden van de HE2 van Spy Fox 1 is mislukt.";
	constexpr auto PATCHER_ERROR_LOAD_SPY_FOX_1_HE0 = "Het laden van de HE0 van Spy Fox 1 is mislukt.";

	constexpr auto PATCHER_ERROR_LOAD_SPY_FOX_2_HE4 = "Het laden van de HE4 van Spy Fox 2 is mislukt.";
	constexpr auto PATCHER_ERROR_LOAD_SPY_FOX_2_A = "Het laden van de (A) van Spy Fox 2 is mislukt.";
	constexpr auto PATCHER_ERROR_LOAD_SPY_FOX_2_HE2 = "Het laden van de HE2 van Spy Fox 2 is mislukt.";
	constexpr auto PATCHER_ERROR_LOAD_SPY_FOX_2_HE0 = "Het laden van de HE0 van Spy Fox 2 is mislukt.";

	constexpr auto PATCHER_ERROR_LOAD_SPY_FOX_3_HE4 = "Het laden van de HE4 van Spy Fox 3 is mislukt.";
	constexpr auto PATCHER_ERROR_LOAD_SPY_FOX_3_A = "Het laden van de (A) van Spy Fox 3 is mislukt.";
	constexpr auto PATCHER_ERROR_LOAD_SPY_FOX_3_HE2 = "Het laden van de HE2 van Spy Fox 3 is mislukt.";
	constexpr auto PATCHER_ERROR_LOAD_SPY_FOX_3_HE0 = "Het laden van de HE0 van Spy Fox 3 is mislukt.";
	
	constexpr auto PATCHER_ERROR_LOAD_SONGS_SPY_FOX_1 = "Er zijn geen songs gevonden in de HE4 van Spy Fox 1.";
	constexpr auto PATCHER_ERROR_LOAD_SONGS_SPY_FOX_2 = "Er zijn geen songs gevonden in de HE4 van Spy Fox 2.";
	constexpr auto PATCHER_ERROR_LOAD_SONGS_SPY_FOX_3 = "Er zijn geen songs gevonden in de HE4 van Spy Fox 3.";

	constexpr auto PATCHER_ERROR_LOAD_BACKGROUNDS_SPY_FOX_1 = "Er is iets misgegaan bij het laden van achtergronden uit de archieven van Spy Fox 1.";
	constexpr auto PATCHER_ERROR_LOAD_BACKGROUNDS_SPY_FOX_2 = "Er is iets misgegaan bij het laden van achtergronden uit de archieven van Spy Fox 2.";
	constexpr auto PATCHER_ERROR_LOAD_BACKGROUNDS_SPY_FOX_3 = "Er is iets misgegaan bij het laden van achtergronden uit de archieven van Spy Fox 3.";
	
	constexpr auto PATCHER_ERROR_LOAD_IMAGES_SPY_FOX_1 = "Er zijn geen afbeeldingen gevonden in de (A) van Spy Fox 1.";
	constexpr auto PATCHER_ERROR_LOAD_IMAGES_SPY_FOX_2 = "Er zijn geen afbeeldingen gevonden in de (A) van Spy Fox 2.";
	constexpr auto PATCHER_ERROR_LOAD_IMAGES_SPY_FOX_3 = "Er zijn geen afbeeldingen gevonden in de (A) van Spy Fox 3.";
	
	constexpr auto PATCHER_ERROR_LOAD_TALKIES_SPY_FOX_1 = "Er is geen spraakdata gevonden in de (A) van Spy Fox 1.";
	constexpr auto PATCHER_ERROR_LOAD_TALKIES_SPY_FOX_2 = "Er is geen spraakdata gevonden in de (A) van Spy Fox 2.";
	constexpr auto PATCHER_ERROR_LOAD_TALKIES_SPY_FOX_3 = "Er is geen spraakdata gevonden in de (A) van Spy Fox 3.";

	constexpr auto PATCHER_ERROR_LOAD_SF_SONG_SPY_FOX_1 = "Het Spy Fox-nummer is niet gevonden in Spy Fox 1.";

	constexpr auto PATCHER_ERROR_LOAD_END_ROOMS_SPY_FOX = "Er zijn geen rooms gevonden in (A) van Spy Fox 1.";
	constexpr auto PATCHER_ERROR_LOAD_END_LFLF_SPY_FOX = "De LFLF is niet gevonden in Spy Fox 1.";
	constexpr auto PATCHER_ERROR_LOAD_END_SOUND_SPY_FOX = "Het eindgeluid is niet gevonden in Spy Fox 1.";

	constexpr auto PATCHER_ERROR_LOAD_ARCHIVES_SPY_FOX_3 = "Het laden van de archieven van Spy Fox 3 is mislukt.";

	constexpr auto PATCHER_ERROR_BIND_SCRIPTS_SPY_FOX_3 = "Het koppelen van de scripts van Spy Fox 3 is mislukt.";
	constexpr auto PATCHER_ERROR_BIND_HE0_SPY_FOX_3 = "Het koppelen van HE0 van Spy Fox 3 is mislukt.";

	constexpr auto PATCHER_ERROR_REPLACING_TALKIES_OUT_OF_RANGE_SPY_FOX_1 = "De spraakdata van Spy Fox 1 bestond niet: ";
	constexpr auto PATCHER_ERROR_REPLACING_TALKIES_OUT_OF_RANGE_SPY_FOX_2 = "De spraakdata van Spy Fox 2 bestond niet: ";
	constexpr auto PATCHER_ERROR_REPLACING_TALKIES_OUT_OF_RANGE_SPY_FOX_3 = "De spraakdata van Spy Fox 3 bestond niet: ";

	constexpr auto PATCHER_ERROR_REPLACING_IMAGES_OUT_OF_RANGE_SPY_FOX_1 = "De afbeelding van Spy Fox 1 bestond niet.";
	constexpr auto PATCHER_ERROR_REPLACING_IMAGES_OUT_OF_RANGE_SPY_FOX_2 = "De afbeelding van Spy Fox 2 bestond niet.";
	constexpr auto PATCHER_ERROR_REPLACING_IMAGES_OUT_OF_RANGE_SPY_FOX_3 = "De afbeelding van Spy Fox 3 bestond niet.";

	constexpr auto PATCHER_ERROR_REPLACING_BACKGROUNDS_OUT_OF_RANGE_SPY_FOX_1 = "De achtergrond van Spy Fox 1 bestond niet.";
	constexpr auto PATCHER_ERROR_REPLACING_BACKGROUNDS_OUT_OF_RANGE_SPY_FOX_2 = "De achtergrond van Spy Fox 2 bestond niet.";
	constexpr auto PATCHER_ERROR_REPLACING_BACKGROUNDS_OUT_OF_RANGE_SPY_FOX_3 = "De achtergrond van Spy Fox 3 bestond niet.";

	constexpr auto PATCHER_ERROR_BUILD_SCRIPTS_SPY_FOX_3 = "Het opbouwen van de scripts van Spy Fox 3 is mislukt.";
	constexpr auto PATCHER_ERROR_BUILD_HE0_SPY_FOX_3 = "Het opbouwen van HE0 van Spy Fox 3 is mislukt.";
}