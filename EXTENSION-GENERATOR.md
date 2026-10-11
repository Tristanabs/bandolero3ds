# Generador de extensiones `.ban3`

El archivo [extension-generator.html](./extension-generator.html) es una herramienta web estática para crear manifiestos de extensiones y temas para Bandolero 3DS.

## Qué genera

La herramienta no genera código ejecutable. Descarga un archivo de texto con extensión `.ban3` que contiene datos:

```ini
id=mi-extension
name=Mi extensión
extension=.ban
theme=Personalizado
background=#0e1322
panel=#192137
accent=#be192d
text=#f4f7ff
cyan=#44d2e0
description=Descripción opcional
```

## Cómo usarlo localmente

1. Abre `extension-generator.html` en un navegador.
2. Escribe un ID usando letras, números, puntos, guiones o guiones bajos.
3. Indica el nombre que aparecerá en “Acerca de”.
4. Elige la extensión de archivo y los colores.
5. Comprueba la vista previa.
6. Pulsa **Descargar extensión .ban3**.
7. Copia el archivo descargado a:

```text
sdmc:/3ds/bandolero/extensions/
```

Reinicia Bandolero 3DS para que vuelva a escanear la carpeta.

## Cómo funciona dentro de Bandolero

Bandolero busca archivos con extensión `.ban3` en `sdmc:/3ds/bandolero/extensions/`. Lee sus pares `clave=valor`, muestra el campo `name` en “Acerca de” y aplica los colores del primer manifiesto válido.

El sistema está limitado intencionadamente a datos. Un `.ban3` no puede instalar, cargar ni ejecutar código nativo.

## Publicar la herramienta con GitHub Pages

### Opción recomendada: repositorio del proyecto

1. Sube `extension-generator.html` al repositorio de GitHub.
2. Opcionalmente renómbralo a `index.html` si quieres que se abra directamente al visitar la web.
3. En GitHub abre **Settings**.
4. En el menú lateral entra en **Pages**.
5. En **Build and deployment**, selecciona:
   - **Source:** `Deploy from a branch`.
   - **Branch:** la rama que contiene el HTML, normalmente `main`.
   - **Folder:** `/ (root)` si el HTML está en la raíz.
6. Pulsa **Save**.
7. Espera a que termine el workflow de Pages.
8. GitHub mostrará la URL pública, normalmente:

```text
https://USUARIO.github.io/REPOSITORIO/
```

Si el archivo se llama `extension-generator.html`, la URL será:

```text
https://USUARIO.github.io/REPOSITORIO/extension-generator.html
```

Si se llama `index.html`, se abrirá directamente con la URL del repositorio.

### Mantener la web actualizada

Cada vez que hagas `push` a la rama y carpeta configuradas en Pages, GitHub volverá a publicar el HTML automáticamente. No necesita servidor, base de datos ni backend: toda la generación ocurre en el navegador del visitante.

## Recomendaciones de publicación

- Mantén el HTML en UTF-8.
- No incluyas claves privadas ni tokens.
- Publica junto al HTML este documento y un enlace al repositorio.
- Explica que los archivos `.ban3` deben copiarse manualmente a la tarjeta SD.
- Revisa los colores en una 3DS real antes de publicar un tema.
