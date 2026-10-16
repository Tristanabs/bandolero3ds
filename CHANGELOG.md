# Registro de cambios

## 2.3.1 (Citro2D) — 2026-10-16

Esta versión moderniza la edición anterior y cambia la presentación principal a Citro2D/Citro3D.

### Interfaz

- Nueva interfaz para las dos pantallas de Nintendo 3DS.
- Editor adaptado a la resolución de la consola con 15 líneas visibles.
- Selección de línea, barra de estado y menú de controles renovados.
- Pantalla de carga y sección “Acerca de” rediseñadas.
- Inclusión del icono, QR y logo de Flagwave Open Code.
- Versión y build visibles en “Acerca de”.

### Archivos y persistencia

- Guardado y apertura en `sdmc:/3ds/bandolero/files/`, incluyendo navegación
  por subcarpetas.
- Creación automática de `files/`, `extensions/` y `config.ini`.
- Persistencia del idioma y del número de arranques.
- Conservación de extensiones de archivo personalizadas.

### Idiomas y texto

- Español, inglés, portugués, francés, alemán y japonés.
- Cambio de idioma desde “Acerca de” mediante `R/L`.
- Fuente RomFS con glifos latinos, símbolos, tildes, `Ñ` y japonés.
- Mejoras en el manejo de texto UTF-8.
- Comprobación automática de secuencias UTF-8 inválidas antes de dibujar.
- Renderizado visual de `**negrita**` y encabezados Markdown.
- Dedicatoria oculta desbloqueable mediante el código Konami.
- La pantalla de “Acerca de” muestra `R/L: IDIOMA` y `X: AVISOS .BAN3`.
- El logo de Flagwave Open Code se coloca en la esquina inferior derecha del
  panel superior.

### Extensiones y temas

- Lectura de manifiestos `.ban3` desde `sdmc:/3ds/bandolero/extensions/`.
- Aplicación de colores de tema desde manifiestos de datos.
- Lista de extensiones cargadas en “Acerca de”.
- Nuevo [generador web](./extension-generator.html).
- Las extensiones no ejecutan código nativo externo.
- `extensions_enabled=0` en `config.ini` desactiva manualmente la carga de
  extensiones; no existe un control equivalente dentro de la app.
- `config.ini` registra actividad con entradas `log=` y purga automáticamente
  los eventos de más de 30 días, incluyendo aperturas, nuevas extensiones y
  actualizaciones de versión.

### CIA y banner

- Metadatos de aplicación actualizados:
  - Nombre: Bandolero 3DS.
  - Descripción: Un editor de texto para 3DS - Version 2.3.1.
  - Editor: Tristanabs.
  - ProductCode: `CTR-P-BNDR`.
  - UniqueId solicitado en el RSF: `000400000FF3FF00`.
- CIA empaquetado pasando explícitamente el SMDH y `banner.bnr` a `makerom`.
- El SMDH contiene el nombre, descripción, autor e icono de Bandolero 3DS.
- `banner.bnr` conserva la imagen y el audio originales del menú HOME.

### Próxima versión — base implementada

- Editor reducido a 15 líneas visibles con desplazamiento y aviso visual cuando hay más contenido.
- Explorador de archivos para abrir documentos de `files/` con cruceta, botones y pantalla táctil.
- El menú inferior reconoce las zonas táctiles de sus acciones principales.
- Apertura y guardado compatibles con formatos de texto y código, incluidos
  `.txt`, `.md`, `.json`, `.xml`, `.html`, `.css`, `.js`, `.c`, `.cpp`, `.py`,
  `.rs`, `.go` y `.sql`.
- Previsualización Markdown básica para encabezados, énfasis y listas.
- Las extensiones siguen siendo manifiestos de datos sin ejecución de código nativo y aceptan descripción.
- Avisos persistentes de nuevas extensiones, con opción para desactivarlos desde “Acerca de”.
- Catálogo web con cinco temas `.ban3` descargables y personalizados.
- Sección “Acerca de” reorganizada en bloques de identidad, enlaces y ajustes, con QR más grande y controles agrupados.
- Se conservan sin cambios el banner, el icono y el audio del menú HOME.

## Versión anterior

La versión anterior estaba basada en la interfaz original de Notepad 3DS y no incluía la nueva pantalla Citro2D, la persistencia de configuración, el sistema `.ban3`, los seis idiomas ni el generador web.
