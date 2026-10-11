# Bandolero 3DS

Editor de texto para Nintendo 3DS, desarrollado por Tristanabs. Bandolero 3DS permite crear, abrir, editar, buscar y guardar archivos de texto directamente en la tarjeta SD.

## Versión actual

- **Versión:** 2.0 (Citro2D)
- **Build documentada:** 2026.10.11
- **Desarrollador:** Tristanabs
- **Identificador solicitado en `homebrew.rsf`:** `000400000FF3FF00`
- **Producto:** `CTR-P-BNDR`

La versión actual utiliza Citro2D/Citro3D, fuente compatible con japonés y soporte UTF-8 para la interfaz y los archivos editados.

## Funciones

- Editor de texto con 19 líneas visibles adaptadas a la resolución de la 3DS.
- Selección de línea mediante la cruceta o el Circle Pad.
- Edición de líneas, búsqueda y salto.
- Guardado y apertura de archivos desde la tarjeta SD.
- Conservación de extensiones personalizadas; si no se indica una, se añade `.txt`.
- Soporte de español, inglés, portugués, francés, alemán y japonés.
- Cambio de idioma desde “Acerca de” usando `R` y `L`.
- Soporte UTF-8, tildes, `Ñ`, símbolos y caracteres japoneses incluidos en la fuente.
- Pantalla “Acerca de” con QR, GitHub, versión, build, logo y extensiones cargadas.
- Temas y extensiones de datos mediante archivos `.ban3`.
- Audio del banner reproducido por el menú HOME de la consola; la aplicación no reproduce música internamente.

## Controles

| Botón | Acción |
| --- | --- |
| `A` | Editar línea |
| `B` | Nuevo archivo |
| `X` | Guardar archivo |
| `Y` | Abrir archivo guardado |
| `R` | Buscar |
| `L` | Saltar |
| Cruceta / Circle Pad | Mover la selección |
| `START` | Salir |
| `SELECT` | Acerca de |

## Datos en la tarjeta SD

En el primer arranque se crean estas rutas:

```text
sdmc:/3ds/bandolero/
sdmc:/3ds/bandolero/files/
sdmc:/3ds/bandolero/extensions/
sdmc:/3ds/bandolero/config.ini
```

Los archivos editados se guardan en `files/`. `config.ini` conserva el idioma elegido y el número de arranques. Las extensiones y temas se colocan en `extensions/`.

## Extensiones `.ban3`

Una extensión es un manifiesto de texto; no contiene ni ejecuta código nativo. Puede declarar:

```ini
id=mi-extension
name=Mi extensión
extension=.md
theme=Personalizado
background=#0e1322
panel=#192137
accent=#be192d
text=#f4f7ff
cyan=#44d2e0
```

La aplicación lee los archivos `.ban3`, muestra sus nombres en “Acerca de” y aplica los colores del primer tema válido. Consulta [EXTENSION-GENERATOR.md](./EXTENSION-GENERATOR.md) para crear y publicar extensiones.

## Compilar

Se necesita devkitPro con devkitARM, libctru, Citro2D/Citro3D y las herramientas 3DS (`makerom` y `bannertool`).

Desde un entorno devkitPro configurado:

```text
make
```

Para generar el perfil Citro2D explícitamente:

```text
make PROFILE=citro2d TARGET=Bandolero3DS-citro2d
```

El `.3dsx` se genera con el `Makefile`. El CIA se empaqueta con `makerom` usando `homebrew.rsf`, el SMDH, `banner.bnr` y el ELF compilado.

## Banner, icono y metadatos

- `icon.png` es la fuente del icono del SMDH.
- `banner.bnr` contiene la imagen del banner y el audio del menú HOME.
- `homebrew.rsf` contiene el título, ProductCode, región y configuración del CIA.
- El audio de un banner HOME debe ser estéreo y durar como máximo 3 segundos.

El audio no se reproduce dentro de Bandolero: lo reproduce el menú HOME cuando la consola selecciona el icono.

## Instalación

Instala el CIA generado con FBI o la aplicación equivalente. Para usar la versión de desarrollo sin instalar un CIA, copia el `.3dsx` a:

```text
sdmc:/3ds/bandolero/
```

## Documentación adicional

- [Registro de cambios](./CHANGELOG.md)
- [Generador `.ban3 (Offline) ](./EXTENSION-GENERATOR.md)
- [Generador web](http://tristanabs.github.io/ban3)

## Créditos

Creado por [Tristanabs](https://github.com/Tristanabs).

Basado en el trabajo original de [Notepad 3DS](https://github.com/RMcTn/Notepad3DS).

Agradecimientos a [Kelonio 3DS](https://www.youtube.com/@Kelonio3DS).

## Soporte

Para soporte, utiliza el servidor Homebrew Central:

<https://dc.gg/hbcentral>

## Flagwave Open Code

Bandolero 3DS está adherido a Flagwave Open Code.

[![Flagwave Open Code](https://i.imgur.com/8OhDyso.png)](https://www.flagwave.es/open-code)
