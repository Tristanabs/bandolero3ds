# Generador de extensiones `.ban3`

El archivo [extension-generator.html](./extension-generator.html) es una herramienta web estática para crear manifiestos de extensiones y temas para Bandolero 3DS.
### [Generador online](https://bandolero.netlify.app/)

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
