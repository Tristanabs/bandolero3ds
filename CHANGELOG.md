# Registro de cambios

## 2.0 (Citro2D) — 2026-10-11

Esta versión moderniza la edición anterior y cambia la presentación principal a Citro2D/Citro3D.

### Interfaz

- Nueva interfaz para las dos pantallas de Nintendo 3DS.
- Editor adaptado a la resolución de la consola con 19 líneas visibles.
- Selección de línea, barra de estado y menú de controles renovados.
- Pantalla de carga y sección “Acerca de” rediseñadas.
- Inclusión del icono, QR y logo de Flagwave Open Code.
- Versión y build visibles en “Acerca de”.

### Archivos y persistencia

- Guardado y apertura en `sdmc:/3ds/bandolero/files/`.
- Creación automática de `files/`, `extensions/` y `config.ini`.
- Persistencia del idioma y del número de arranques.
- Conservación de extensiones de archivo personalizadas.

### Idiomas y texto

- Español, inglés, portugués, francés, alemán y japonés.
- Cambio de idioma desde “Acerca de” mediante `R/L`.
- Fuente RomFS con glifos latinos, símbolos, tildes, `Ñ` y japonés.
- Mejoras en el manejo de texto UTF-8.

### Extensiones y temas

- Lectura de manifiestos `.ban3` desde `sdmc:/3ds/bandolero/extensions/`.
- Aplicación de colores de tema desde manifiestos de datos.
- Lista de extensiones cargadas en “Acerca de”.
- Nuevo [generador web](./extension-generator.html).
- Las extensiones no ejecutan código nativo externo.

### CIA y banner

- Metadatos de aplicación actualizados:
  - Nombre: Bandolero 3DS.
  - Descripción: Un editor de texto para 3DS - Version 2.0.
  - Editor: Tristanabs.
  - ProductCode: `CTR-P-BNDR`.
  - UniqueId solicitado en el RSF: `000400000FF3FF00`.
- Banner con icono, imagen y audio del menú HOME.
- Audio convertido a `CWAV` estéreo de menos de 3 segundos para compatibilidad con HOME.

## Versión anterior

La versión anterior estaba basada en la interfaz original de Notepad 3DS y no incluía la nueva pantalla Citro2D, la persistencia de configuración, el sistema `.ban3`, los seis idiomas ni el generador web.
