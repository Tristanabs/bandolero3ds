# Generador de Extensiones .ban3

<p align="center">
  <!-- Badge: Web Online -->
  <a href="https://bandolero.flagwave.es">
    <img src="https://img.shields.io/badge/WEB-BANDOLERO.FLAGWAVE.ES-0070f3?style=for-the-badge&logo=googlechrome&logoColor=white" alt="Acceder a la Web" />
  </a>

  <!-- Badge: Flagwave Open Code -->
  <a href="https://www.flagwave.es/open-code">
    <img src="https://img.shields.io/badge/FLAGWAVE-OPEN%20CODE-e63333?style=for-the-badge&logo=data:image/svg%2bxml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHZpZXdCb3g9IjAgMCAyNCAyNCIgZmlsbD0id2hpdGUiPjxwYXRoIGQ9Ik0zIDNoM3YxOEgzVjN6bTUtMmgzdjNoLTNWM3ptNSAwaDN2M2gtM1Yzem0tNSA1aDN2MTBoLTNWOXptNSA1aDN2NWgtM1YxNHoiLz48L3N2Zz4=&labelColor=1a1a1a" alt="Flagwave Open Code" />
  </a>

  <!-- Badge: Plataforma HTML5 -->
  <img src="https://img.shields.io/badge/PLATAFORMA-HTML5-E34F26?style=for-the-badge&logo=html5&logoColor=white" alt="Plataforma HTML5" />

  <!-- Badge: Made with Vercel -->
  <a href="https://vercel.com">
    <img src="https://img.shields.io/badge/MADE%20WITH-VERCEL-000000?style=for-the-badge&logo=vercel&logoColor=white" alt="Made with Vercel" />
  </a>
</p>

---

## Generador Online

Accesible siempre en: **[bandolero.flagwave.es](https://bandolero.flagwave.es)**

---

## Qué genera

El archivo [`extension-generator.html`](./extension-generator.html) es una herramienta web estática diseñada para crear manifiestos de extensiones y temas para **Bandolero 3DS**.

La herramienta **no genera código ejecutable**. Descarga un archivo de texto plano con extensión `.ban3` que contiene únicamente datos de configuración estructurados en pares `clave=valor`:

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
````
# Cómo usarlo localmente
1. Abre extension-generator.html en cualquier navegador web.
2. Escribe un ID utilizando únicamente letras, números, puntos, guiones o guiones bajos.
3. Indica el nombre que aparecerá en la sección "Acerca de".
4. Elige la extensión de archivo por defecto y personaliza la paleta de colores.
5. Comprueba la apariencia visual en la vista previa interactiva.
6. Pulsa Descargar extensión .ban3.
7. Copia el archivo descargado a la siguiente ruta en la tarjeta SD de tu Nintendo 3DS: ````sdmc:/3ds/bandolero/extensions/````

Una vez copiado el archivo en la tarjeta SD, reinicia Bandolero 3DS para que la aplicación vuelva a escanear la carpeta de extensiones automáticamente.

# Cómo funciona dentro de Bandolero
Al iniciarse, Bandolero 3DS busca archivos con extensión .ban3 en el directorio sdmc:/3ds/bandolero/extensions/. Durante el escaneo realiza el siguiente proceso:

Lee los pares clave=valor contenidos en cada archivo manifiesto .ban3.

Muestra el campo name dentro de la sección "Acerca de".

Conserva el campo description para mostrarlo en las pantallas informativas de la consola.

Aplica el esquema de colores del primer manifiesto válido que encuentre en la carpeta.

# Tienda integrada y catálogo estático
La página web también incluye una tienda estática integrada. El catálogo se mantiene manualmente en el array extensionCatalog dentro de extension-generator.html: cada entrada debe tener un nombre, una descripción y un enlace directo a un archivo .ban3.


Aislamiento y Seguridad:

El sistema está limitado intencionadamente a datos. Un archivo .ban3 no puede instalar, cargar ni ejecutar código nativo bajo ninguna circunstancia.
