#include "localization.h"

namespace { AppLanguage current_language = APP_LANGUAGE_ENGLISH; }

void localization_init(AppLanguage configured_language) {
    current_language = configured_language;
}
AppLanguage localization_language() { return current_language; }
void localization_set_language(AppLanguage language) { current_language = language; }

const char* language_name(AppLanguage language) {
    static const char* names[] = {"ESPANOL", "ENGLISH", "PORTUGUES", "FRANCAIS",
                                  "DEUTSCH", "日本語"};
    return names[static_cast<int>(language)];
}

const char* tr(TextId id) {
    const bool spanish = current_language == APP_LANGUAGE_SPANISH;
    const bool portuguese = current_language == APP_LANGUAGE_PORTUGUESE;
    const bool french = current_language == APP_LANGUAGE_FRENCH;
    const bool german = current_language == APP_LANGUAGE_GERMAN;
    const bool japanese = current_language == APP_LANGUAGE_JAPANESE;

    if (spanish) {
        switch (id) {
        case TEXT_APP_TITLE: return "BANDOLERO 3DS";
        case TEXT_APP_SUBTITLE: return "UN EDITOR DE TEXTO PARA 3DS";
        case TEXT_MENU: return "MENU";
        case TEXT_EDIT: return "A  EDITAR LINEA";
        case TEXT_NEW_FILE: return "B  NUEVO ARCHIVO";
        case TEXT_SAVE_FILE: return "X  GUARDAR ARCHIVO";
        case TEXT_OPEN_FILE: return "Y  ABRIR ARCHIVO";
        case TEXT_SEARCH: return "R  BUSCAR";
        case TEXT_JUMP: return "L  SALTAR";
        case TEXT_MOVE: return "DPAD / CPAD  MOVER";
        case TEXT_EXIT: return "START  SALIR";
        case TEXT_ABOUT: return "SELECT  ACERCA DE";
        case TEXT_ACTIONS: return "ACCIONES";
        case TEXT_EMPTY_LINE: return "(linea vacia)";
        case TEXT_READY: return "LISTO";
        case TEXT_NO_FILE: return "SIN ARCHIVO";
        case TEXT_ABOUT_TITLE: return "ACERCA DE";
        case TEXT_DEVELOPER: return "DESARROLLADOR: TRISTANABS";
        case TEXT_GITHUB: return "github.com/Tristanabs/bandolero3ds";
        case TEXT_COPYRIGHT: return "(C) 2026 TRISTANABS";
        case TEXT_LANGUAGE: return "IDIOMA";
        case TEXT_LANGUAGE_CHANGE: return "R/L CAMBIAR IDIOMA";
        case TEXT_LANGUAGE_PROMPT: return "ELIGE EL IDIOMA";
        case TEXT_LANGUAGE_HELP: return "R/L CAMBIAR   A ACEPTAR";
        case TEXT_LOADING: return "CARGANDO";
        case TEXT_LOADING_SUBTITLE: return "UN EDITOR DE TEXTO PARA 3DS";
        case TEXT_LOADING_VERSION: return "VERSION 2.0 (CITRO2D)";
        case TEXT_FIRST_RUN_TITLE: return "PRIMER ARRANQUE";
        case TEXT_FIRST_RUN_LINE_1: return "Los archivos se guardan en";
        case TEXT_FIRST_RUN_LINE_2: return "3ds/bandolero/files en formato TXT.";
        case TEXT_FIRST_RUN_LINE_3: return "Extensiones y temas: 3ds/bandolero/extensions";
        case TEXT_FIRST_RUN_CONTINUE: return "Pulsa A para continuar";
        case TEXT_VERSION: return "Bandolero 3DS version 2.0";
        case TEXT_EDIT_HINT: return "Introduce el texto aqui.";
        case TEXT_NEW_CONFIRM: return "Crear un archivo en blanco? y/n";
        case TEXT_SEARCH_HINT: return "Introduce el termino de busqueda";
        case TEXT_FILENAME_HINT: return "Introduce el nombre del archivo.";
        case TEXT_NEW_CREATED: return "Archivo nuevo creado";
        case TEXT_NEW_CANCELLED: return "No se creo ningun archivo nuevo";
        case TEXT_SEARCH_NOT_FOUND: return "No se encontro: %s";
        case TEXT_SEARCH_FOUND: return "Encontrado %s en linea %d";
        case TEXT_SAVE_OK: return "Archivo guardado en %s";
        case TEXT_SAVE_FAILED: return "No se pudo guardar %s";
        case TEXT_OPEN_OK: return "Archivo abierto correctamente: %s";
        case TEXT_OPEN_FAILED: return "No se pudo abrir: %s";
        case TEXT_CURRENT_LINE: return "Linea actual: %u";
        case TEXT_CURRENT_FILE: return "Archivo actual: %s";
        }
    } else if (portuguese) {
        switch (id) {
        case TEXT_APP_TITLE: return "BANDOLERO 3DS"; case TEXT_APP_SUBTITLE: return "EDITOR DE TEXTO PARA 3DS";
        case TEXT_MENU: return "MENU"; case TEXT_EDIT: return "A  EDITAR LINHA"; case TEXT_NEW_FILE: return "B  NOVO ARQUIVO";
        case TEXT_SAVE_FILE: return "X  GUARDAR ARQUIVO"; case TEXT_OPEN_FILE: return "Y  ABRIR ARQUIVO"; case TEXT_SEARCH: return "R  PESQUISAR";
        case TEXT_JUMP: return "L  SALTAR"; case TEXT_MOVE: return "DPAD / CPAD  MOVER"; case TEXT_EXIT: return "START  SAIR"; case TEXT_ABOUT: return "SELECT  SOBRE";
        case TEXT_ACTIONS: return "ACOES"; case TEXT_EMPTY_LINE: return "(linha vazia)"; case TEXT_READY: return "PRONTO"; case TEXT_NO_FILE: return "SEM ARQUIVO";
        case TEXT_ABOUT_TITLE: return "SOBRE"; case TEXT_DEVELOPER: return "DESENVOLVEDOR: TRISTANABS"; case TEXT_GITHUB: return "github.com/Tristanabs/bandolero3ds";
        case TEXT_COPYRIGHT: return "(C) 2026 TRISTANABS"; case TEXT_LANGUAGE: return "IDIOMA"; case TEXT_LANGUAGE_PROMPT: return "ESCOLHA O IDIOMA";
        case TEXT_LANGUAGE_CHANGE: return "R/L MUDAR IDIOMA";
        case TEXT_LOADING: return "A CARREGAR"; case TEXT_LOADING_SUBTITLE: return "EDITOR DE TEXTO PARA 3DS"; case TEXT_LOADING_VERSION: return "VERSAO 2.0 (CITRO2D)";
        case TEXT_LANGUAGE_HELP: return "R/L MUDAR   A ACEITAR"; case TEXT_FIRST_RUN_TITLE: return "PRIMEIRO ARRANQUE";
        case TEXT_FIRST_RUN_LINE_1: return "Os arquivos ficam em"; case TEXT_FIRST_RUN_LINE_2: return "3ds/bandolero/files em formato TXT.";
        case TEXT_FIRST_RUN_LINE_3: return "Extensoes e temas: 3ds/bandolero/extensions"; case TEXT_FIRST_RUN_CONTINUE: return "Prima A para continuar";
        default: break;
        }
    } else if (french) {
        switch (id) {
        case TEXT_APP_TITLE: return "BANDOLERO 3DS"; case TEXT_APP_SUBTITLE: return "EDITEUR DE TEXTE POUR 3DS"; case TEXT_MENU: return "MENU";
        case TEXT_EDIT: return "A  MODIFIER LA LIGNE"; case TEXT_NEW_FILE: return "B  NOUVEAU FICHIER"; case TEXT_SAVE_FILE: return "X  ENREGISTRER";
        case TEXT_OPEN_FILE: return "Y  OUVRIR"; case TEXT_SEARCH: return "R  RECHERCHER"; case TEXT_JUMP: return "L  SAUTER"; case TEXT_MOVE: return "DPAD / CPAD  DEPLACER";
        case TEXT_EXIT: return "START  QUITTER"; case TEXT_ABOUT: return "SELECT  A PROPOS"; case TEXT_ACTIONS: return "ACTIONS"; case TEXT_READY: return "PRET";
        case TEXT_EMPTY_LINE: return "(ligne vide)"; case TEXT_NO_FILE: return "SANS FICHIER"; case TEXT_ABOUT_TITLE: return "A PROPOS"; case TEXT_DEVELOPER: return "DEVELOPPEUR: TRISTANABS";
        case TEXT_GITHUB: return "github.com/Tristanabs/bandolero3ds"; case TEXT_COPYRIGHT: return "(C) 2026 TRISTANABS"; case TEXT_LANGUAGE: return "LANGUE";
        case TEXT_LANGUAGE_CHANGE: return "R/L CHANGER LANGUE";
        case TEXT_LOADING: return "CHARGEMENT"; case TEXT_LOADING_SUBTITLE: return "EDITEUR DE TEXTE POUR 3DS"; case TEXT_LOADING_VERSION: return "VERSION 2.0 (CITRO2D)";
        case TEXT_LANGUAGE_PROMPT: return "CHOISISSEZ LA LANGUE"; case TEXT_LANGUAGE_HELP: return "R/L CHANGER   A VALIDER"; case TEXT_FIRST_RUN_TITLE: return "PREMIER DEMARRAGE";
        case TEXT_FIRST_RUN_LINE_1: return "Les fichiers sont dans"; case TEXT_FIRST_RUN_LINE_2: return "3ds/bandolero/files au format TXT.";
        case TEXT_FIRST_RUN_LINE_3: return "Extensions et themes: 3ds/bandolero/extensions"; case TEXT_FIRST_RUN_CONTINUE: return "Appuyez sur A pour continuer";
        case TEXT_VERSION: return "Bandolero 3DS version 2.0";
        case TEXT_EDIT_HINT: return "Saisissez le texte.";
        case TEXT_NEW_CONFIRM: return "Creer un fichier vide ? y/n";
        case TEXT_SEARCH_HINT: return "Saisissez le terme a rechercher";
        case TEXT_FILENAME_HINT: return "Saisissez le nom du fichier.";
        case TEXT_NEW_CREATED: return "Nouveau fichier cree";
        case TEXT_NEW_CANCELLED: return "Creation du fichier annulee";
        case TEXT_SEARCH_NOT_FOUND: return "Introuvable : %s";
        case TEXT_SEARCH_FOUND: return "Trouve %s a la ligne %d";
        case TEXT_SAVE_OK: return "Fichier enregistre dans %s";
        case TEXT_SAVE_FAILED: return "Echec de l'enregistrement : %s";
        case TEXT_OPEN_OK: return "Fichier ouvert : %s";
        case TEXT_OPEN_FAILED: return "Echec de l'ouverture : %s";
        case TEXT_CURRENT_LINE: return "Ligne actuelle : %u";
        case TEXT_CURRENT_FILE: return "Fichier actuel : %s";
        }
    } else if (german) {
        switch (id) {
        case TEXT_APP_TITLE: return "BANDOLERO 3DS"; case TEXT_APP_SUBTITLE: return "TEXTEDITOR FUR 3DS"; case TEXT_MENU: return "MENU";
        case TEXT_EDIT: return "A  ZEILE EDITIEREN"; case TEXT_NEW_FILE: return "B  NEUE DATEI"; case TEXT_SAVE_FILE: return "X  DATEI SPEICHERN";
        case TEXT_OPEN_FILE: return "Y  DATEI OFFNEN"; case TEXT_SEARCH: return "R  SUCHEN"; case TEXT_JUMP: return "L  SPRINGEN"; case TEXT_MOVE: return "DPAD / CPAD  BEWEGEN";
        case TEXT_EXIT: return "START  BEENDEN"; case TEXT_ABOUT: return "SELECT  INFO"; case TEXT_ACTIONS: return "AKTIONEN"; case TEXT_READY: return "BEREIT";
        case TEXT_EMPTY_LINE: return "(leere Zeile)"; case TEXT_NO_FILE: return "KEINE DATEI"; case TEXT_ABOUT_TITLE: return "INFO"; case TEXT_DEVELOPER: return "ENTWICKLER: TRISTANABS";
        case TEXT_GITHUB: return "github.com/Tristanabs/bandolero3ds"; case TEXT_COPYRIGHT: return "(C) 2026 TRISTANABS"; case TEXT_LANGUAGE: return "SPRACHE";
        case TEXT_LANGUAGE_CHANGE: return "R/L SPRACHE WECHSELN";
        case TEXT_LOADING: return "LADEN"; case TEXT_LOADING_SUBTITLE: return "TEXTEDITOR FUR 3DS"; case TEXT_LOADING_VERSION: return "VERSION 2.0 (CITRO2D)";
        case TEXT_LANGUAGE_PROMPT: return "SPRACHE WAEHLEN"; case TEXT_LANGUAGE_HELP: return "R/L WECHSELN   A OK"; case TEXT_FIRST_RUN_TITLE: return "ERSTER START";
        case TEXT_FIRST_RUN_LINE_1: return "Dateien werden gespeichert in"; case TEXT_FIRST_RUN_LINE_2: return "3ds/bandolero/files als TXT.";
        case TEXT_FIRST_RUN_LINE_3: return "Erweiterungen/Themen: 3ds/bandolero/extensions"; case TEXT_FIRST_RUN_CONTINUE: return "A druecken zum Fortfahren";
        case TEXT_VERSION: return "Bandolero 3DS Version 2.0";
        case TEXT_EDIT_HINT: return "Text eingeben.";
        case TEXT_NEW_CONFIRM: return "Leere Datei erstellen? y/n";
        case TEXT_SEARCH_HINT: return "Suchbegriff eingeben";
        case TEXT_FILENAME_HINT: return "Dateinamen eingeben.";
        case TEXT_NEW_CREATED: return "Neue Datei erstellt";
        case TEXT_NEW_CANCELLED: return "Erstellung abgebrochen";
        case TEXT_SEARCH_NOT_FOUND: return "Nicht gefunden: %s";
        case TEXT_SEARCH_FOUND: return "%s in Zeile %d gefunden";
        case TEXT_SAVE_OK: return "Datei gespeichert: %s";
        case TEXT_SAVE_FAILED: return "Speichern fehlgeschlagen: %s";
        case TEXT_OPEN_OK: return "Datei geoeffnet: %s";
        case TEXT_OPEN_FAILED: return "Oeffnen fehlgeschlagen: %s";
        case TEXT_CURRENT_LINE: return "Aktuelle Zeile: %u";
        case TEXT_CURRENT_FILE: return "Aktuelle Datei: %s";
        }
    } else if (japanese) {
            switch (id) {
            case TEXT_APP_TITLE: return "BANDOLERO 3DS"; case TEXT_APP_SUBTITLE: return "3DS テキストエディター"; case TEXT_MENU: return "メニュー";
            case TEXT_EDIT: return "A  行を編集"; case TEXT_NEW_FILE: return "B  新規ファイル"; case TEXT_SAVE_FILE: return "X  保存";
            case TEXT_OPEN_FILE: return "Y  開く"; case TEXT_SEARCH: return "R  検索"; case TEXT_JUMP: return "L  移動"; case TEXT_MOVE: return "DPAD / CPAD  移動";
            case TEXT_EXIT: return "START  終了"; case TEXT_ABOUT: return "SELECT  情報"; case TEXT_ACTIONS: return "操作"; case TEXT_READY: return "準備完了";
            case TEXT_EMPTY_LINE: return "(空の行)"; case TEXT_NO_FILE: return "ファイルなし"; case TEXT_ABOUT_TITLE: return "情報"; case TEXT_DEVELOPER: return "開発者: TRISTANABS";
            case TEXT_GITHUB: return "github.com/Tristanabs/bandolero3ds"; case TEXT_COPYRIGHT: return "(C) 2026 TRISTANABS"; case TEXT_LANGUAGE: return "言語";
            case TEXT_LANGUAGE_CHANGE: return "R/L 言語を変更";
            case TEXT_LOADING: return "読み込み中"; case TEXT_LOADING_SUBTITLE: return "3DS テキストエディター"; case TEXT_LOADING_VERSION: return "バージョン 2.0 (CITRO2D)";
            case TEXT_LANGUAGE_PROMPT: return "言語を選択"; case TEXT_LANGUAGE_HELP: return "R/L 変更   A 決定"; case TEXT_FIRST_RUN_TITLE: return "初回起動";
            case TEXT_FIRST_RUN_LINE_1: return "ファイルの保存先:"; case TEXT_FIRST_RUN_LINE_2: return "3ds/bandolero/files (TXT)";
            case TEXT_FIRST_RUN_LINE_3: return "拡張機能/テーマ: 3ds/bandolero/extensions"; case TEXT_FIRST_RUN_CONTINUE: return "Aで続行";
            default: break;
            }
        }

    switch (id) {
    case TEXT_APP_TITLE: return "BANDOLERO 3DS"; case TEXT_APP_SUBTITLE: return "A TEXT EDITOR FOR 3DS"; case TEXT_MENU: return "MENU";
    case TEXT_EDIT: return "A  EDIT LINE"; case TEXT_NEW_FILE: return "B  NEW FILE"; case TEXT_SAVE_FILE: return "X  SAVE FILE"; case TEXT_OPEN_FILE: return "Y  OPEN FILE";
    case TEXT_SEARCH: return "R  SEARCH"; case TEXT_JUMP: return "L  JUMP"; case TEXT_MOVE: return "DPAD / CPAD  MOVE"; case TEXT_EXIT: return "START  EXIT"; case TEXT_ABOUT: return "SELECT  ABOUT";
    case TEXT_ACTIONS: return "ACTIONS"; case TEXT_EMPTY_LINE: return "(empty line)"; case TEXT_READY: return "READY"; case TEXT_NO_FILE: return "NO FILE"; case TEXT_ABOUT_TITLE: return "ABOUT";
    case TEXT_DEVELOPER: return "DEVELOPER: TRISTANABS"; case TEXT_GITHUB: return "github.com/Tristanabs/bandolero3ds"; case TEXT_COPYRIGHT: return "(C) 2026 TRISTANABS";
    case TEXT_LANGUAGE_CHANGE: return "R/L CHANGE LANGUAGE";
    case TEXT_LOADING: return "LOADING";
    case TEXT_LOADING_SUBTITLE: return "A TEXT EDITOR FOR 3DS";
    case TEXT_LOADING_VERSION: return "VERSION 2.0 (CITRO2D)";
    case TEXT_LANGUAGE: return "LANGUAGE"; case TEXT_LANGUAGE_PROMPT: return "CHOOSE LANGUAGE"; case TEXT_LANGUAGE_HELP: return "R/L CHANGE   A ACCEPT";
    case TEXT_FIRST_RUN_TITLE: return "FIRST START"; case TEXT_FIRST_RUN_LINE_1: return "Files are saved in"; case TEXT_FIRST_RUN_LINE_2: return "3ds/bandolero/files as TXT.";
    case TEXT_FIRST_RUN_LINE_3: return "Extensions/themes: 3ds/bandolero/extensions"; case TEXT_FIRST_RUN_CONTINUE: return "Press A to continue";
    case TEXT_VERSION: return "Bandolero 3DS version 2.0";
    case TEXT_EDIT_HINT: return "Enter text here.";
    case TEXT_NEW_CONFIRM: return "Create a blank file? y/n";
    case TEXT_SEARCH_HINT: return "Enter search term";
    case TEXT_FILENAME_HINT: return "Enter the file name.";
    case TEXT_NEW_CREATED: return "New file created";
    case TEXT_NEW_CANCELLED: return "New file was not created";
    case TEXT_SEARCH_NOT_FOUND: return "Not found: %s";
    case TEXT_SEARCH_FOUND: return "Found %s on line %d";
    case TEXT_SAVE_OK: return "File saved to %s";
    case TEXT_SAVE_FAILED: return "Could not save %s";
    case TEXT_OPEN_OK: return "File opened: %s";
    case TEXT_OPEN_FAILED: return "Could not open: %s";
    case TEXT_CURRENT_LINE: return "Current line: %u";
    case TEXT_CURRENT_FILE: return "Current file: %s";
    }
    return "";
}
