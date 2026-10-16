# Bandolero 3DS

Editor de texto para Nintendo 3DS, desarrollado por Tristanabs. Bandolero 3DS permite crear, abrir, editar, buscar y guardar archivos de texto directamente en la tarjeta SD.
<p align="center">
  <!-- Badge: Versión Actual -->
  <a href="https://github.com/Tristanabs/bandolero3ds/releases/latest">
    <img src="https://img.shields.io/github/v/release/Tristanabs/bandolero3ds?style=for-the-badge&color=0070f3&label=VERSI%C3%93N%20ACTUAL&logo=github" alt="Versión Actual" />
  </a>

  <!-- Badge: Universal-DB -->
  <a href="https://db.universal-team.net/3ds/bandolero-3ds">
    <img src="https://img.shields.io/badge/UNIVERSAL--DB-VER%20EN%20DB-00D1B2?style=for-the-badge&logo=nintendo-3ds&logoColor=white" alt="Ver en Universal-DB" />
  </a>

  <!-- Badge: Descarga por QR -->
  <a href="#-instalación-rápida-mediante-qr-fbi">
    <img src="https://img.shields.io/badge/DESCARGA%20POR-QR%20CODE-E4000F?style=for-the-badge&logo=qr-code&logoColor=white" alt="Descarga por QR" />
  </a>

  <!-- Badge: Discord HB Central -->
  <a href="https://dc.gg/hbcentral">
    <img src="https://img.shields.io/badge/DISCORD-HB%20CENTRAL-5865F2?style=for-the-badge&logo=discord&logoColor=white" alt="Discord HB Central" />
  </a>

  <!-- Badge: Plataforma 3DS / 2DS -->
  <img src="https://img.shields.io/badge/PLATAFORMA-NINTENDO%203DS%20%2F%202DS-FF0000?style=for-the-badge&logo=nintendo3ds&logoColor=white" alt="Plataforma Nintendo 3DS" />

  <!-- Badge: Licencia MIT -->
  <a href="LICENSE">
    <img src="https://img.shields.io/github/license/Tristanabs/bandolero3ds?style=for-the-badge&color=orange&label=LICENCIA" alt="Licencia" />
  </a>

<!-- Badge: Flagwave Open Code con Isotipo Vectorial Integrado -->
  <a href="https://www.flagwave.es/open-code">
    <img src="https://img.shields.io/badge/FLAGWAVE-OPEN%20CODE-e63333?style=for-the-badge&logo=data:image/svg%2bxml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHZpZXdCb3g9IjAgMCAyNCAyNCIgZmlsbD0id2hpdGUiPjxwYXRoIGQ9Ik0zIDNoM3YxOEgzVjN6bTUtMmgzdjNoLTNWM3ptNSAwaDN2M2gtM1Yzem0tNSA1aDN2MTBoLTNWOXptNSA1aDN2NWgtM1YxNHoiLz48L3N2Zz4=&labelColor=1a1a1a" alt="Flagwave Open Code" />
  </a>
  
## Instalación rápida mediante QR (FBI)

Escanea el siguiente código QR desde la aplicación **FBI** en tu Nintendo 3DS (`Remote Install` -> `Scan QR Code`):

<p align="center">
  <img src="https://api.qrserver.com/v1/create-qr-code/?size=250x250&data=https://github.com/Tristanabs/bandolero3ds/releases/latest/download/bandolero3ds.cia" alt="Código QR de instalación para FBI" width="200" />
</p>

## Versión actual

- **Versión:** 2.3.1 (Citro2D)
- **Build documentada:** 2026.10.16
- **Desarrollador:** Tristanabs
- **Identificador solicitado en `homebrew.rsf`:** `000400000FF3FF00`
- **Producto:** `CTR-P-BNDR`

La versión actual utiliza Citro2D/Citro3D, fuente compatible con japonés y soporte UTF-8 para la interfaz y los archivos editados.

## Funciones

- Editor de texto con 15 líneas visibles y desplazamiento por bloques.
- Selección de línea mediante la cruceta o el Circle Pad.
- Edición de líneas, búsqueda y salto.
- Guardado y apertura de archivos desde la tarjeta SD.
- Explorador de archivos para seleccionar documentos con cruceta, botones o táctil,
  entrar en subcarpetas y volver a la carpeta superior.
- Conservación de extensiones personalizadas; si no se indica una, se añade `.txt`.
- Compatible con documentos de texto y código (`.txt`, `.md`, `.json`, `.xml`,
  `.html`, `.css`, `.js`, `.c`, `.cpp`, `.h`, `.py`, `.rs`, `.go`, `.sql` y
  otros formatos de texto).
- Previsualización Markdown básica para encabezados, listas y `**negrita**`.
- Soporte de español, inglés, portugués, francés, alemán y japonés.
- Cambio de idioma desde “Acerca de” usando `R` y `L`.
- Soporte UTF-8, tildes, `Ñ`, símbolos y caracteres japoneses incluidos en la
  fuente, con comprobación automática de secuencias UTF-8 inválidas.
- Pantalla “Acerca de” reorganizada por bloques: identidad, versión, QR, GitHub, idioma, extensiones y ajustes.
- La dedicatoria está oculta y se desbloquea con el código Konami:
  `↑ ↑ ↓ ↓ ← → ← → B A`.
- Aviso de nuevas extensiones con opción persistente para desactivarlo desde “Acerca de”.
- El texto `X: AVISOS .BAN3` identifica claramente ese control.
- Temas y extensiones de datos mediante archivos `.ban3`.
- Las acciones principales del menú inferior también se pueden activar desde la pantalla táctil.
- Audio del banner reproducido por el menú HOME de la consola; la aplicación no reproduce música internamente.
- Catálogo web con cinco temas `.ban3`: Aurora Boreal, Arcade Sunset, Bosque
  Pixel, Océano Profundo y Brasa.

## Controles

| Botón | Acción |
| --- | --- |
| `A` | Editar línea |
| `B` | Nuevo archivo |
| `X` | Guardar archivo |
| `Y` | Abrir archivo desde el explorador |
| `R` | Buscar |
| `L` | Saltar |
| Cruceta / Circle Pad | Mover la selección |
| `START` | Salir |
| `SELECT` | Acerca de |

En “Acerca de”, `X: AVISOS .BAN3` activa o desactiva los avisos de nuevas
extensiones. La preferencia se guarda en `config.ini`. `R/L: IDIOMA` cambia
el idioma de la interfaz.

## Datos en la tarjeta SD

En el primer arranque se crean estas rutas:

```text
sdmc:/3ds/bandolero/
sdmc:/3ds/bandolero/files/
sdmc:/3ds/bandolero/extensions/
sdmc:/3ds/bandolero/config.ini
```

Los archivos editados se guardan en `files/`. `config.ini` conserva el idioma elegido y el número de arranques. Las extensiones y temas se colocan en `extensions/`. El explorador muestra archivos y subcarpetas, permite entrar y volver sin introducir manualmente el nombre.

`config.ini` admite la opción manual `extensions_enabled=0` para desactivar
completamente la carga de extensiones `.ban3`; la aplicación no ofrece un
botón para cambiar esta opción. El valor `extensions_enabled=1` la activa de
nuevo. El mismo archivo conserva entradas `log=` con la actividad de la app:
arranques, nuevas extensiones, actualizaciones y aperturas de archivos. Solo
se conservan los últimos 30 días.

Ejemplo:

```ini
extensions_enabled=0
```

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

En Windows, el instalador debe dejar estas rutas al principio del `PATH`:

```text
C:\devkitPro\msys2\usr\bin
C:\devkitPro\devkitARM\bin
C:\devkitPro\tools\bin
```

Y estas variables:

```text
DEVKITPRO=C:\devkitPro
DEVKITARM=C:\devkitPro\devkitARM
CTRULIB=C:\devkitPro\libctru
```

Para generar el perfil Citro2D explícitamente:

```text
make PROFILE=citro2d TARGET=Bandolero3DS-citro2d
```

El `.3dsx` se genera con el `Makefile`. Para que el CIA conserve correctamente
el nombre, icono, banner y audio, hay que pasar explícitamente el SMDH y el
banner a `makerom`:

```text
makerom -f cia -o Bandolero3DS-next-2.cia -rsf homebrew.rsf -target t -exefslogo -elf Bandolero3DS-next-2.elf -icon Bandolero3DS-next-2.smdh -banner banner.bnr
```

## Banner, icono y metadatos

- `icon.png` es la fuente del icono del SMDH.
- `banner.bnr` contiene la imagen del banner y el audio del menú HOME.
- `homebrew.rsf` contiene ProductCode, UniqueId y configuración del CIA.
- El título, descripción, autor e icono del SMDH se generan desde `Makefile`.
- El audio de un banner HOME debe ser estéreo y durar como máximo 3 segundos.

El audio no se reproduce dentro de Bandolero: lo reproduce el menú HOME cuando la consola selecciona el icono.

## Instalación

Instala el CIA generado con FBI o la aplicación equivalente. Para usar la versión de desarrollo sin instalar un CIA, copia el `.3dsx` a:

```text
sdmc:/3ds/bandolero/
```

## Documentación adicional

- [Registro de cambios](./CHANGELOG.md)
- [Generador `.ban3` ](./EXTENSION-GENERATOR.md)
- [Generador web](https://bandolero.flagwave.es/)

##  Créditos y Agradecimientos

*  **Desarrollado por:** [Tristanabs](https://github.com/Tristanabs)
*  **Basado originalmente en:** [Notepad 3DS](https://github.com/RMcTn/Notepad3DS) por [RMcTn](https://github.com/RMcTn)
*  **Agradecimientos especiales:** [Kelonio 3DS](https://www.youtube.com/@Kelonio3DS)

---

##  Soporte y Comunidad

Para cualquier duda, asistencia técnica o reporte de errores, visita la comunidad oficial de **Homebrew Central**:

<p align="center">
  <a href="https://dc.gg/hbcentral">
    <img src="https://img.shields.io/badge/DISCORD-UNIRSE%20A%20HB%20CENTRAL-5865F2?style=for-the-badge&logo=discord&logoColor=white" alt="Discord HB Central" />
  </a>
</p>
