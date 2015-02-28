#!/bin/bash
# ##############################################################################
# Nombre      : saii.sh
# Finalidad   : script para el envío de trabajos a las colas de tajinaste.saii.ull.es
# Modo de uso : para enviar el trabajo a la cola hacer:
#                         > qsub script.sh
# ##############################################################################
# ##############################################################################
# Modificar los parámetros correspondientes tal y como sea necesario.
# Aquellas opciones que no sean necesarias pueden comentarse incluyendo una
# almohadilla extra al comienzo de la línea correspondiente.
# ##############################################################################
# Asigna un nombre al trabajo. Por defecto, es el nombre del script de trabajo PBS.
#PBS -N ptop-pavel
# Número de nodos y de procesadores por nodo que se solicitan.
#PBS -l nodes=1:ppn=2
# Nombre de la cola a la que se desea enviar el trabajo (batch).
#PBS -q batch
# Tiempo máximo total durante el cual el trabajo puede estar ejecutándose (horas:min:seg).
#PBS -l walltime=00:15:00
# Ruta y nombre del fichero al que se redirige la salida estándar.
#PBS -o ptop.out
# Ruta y nombre del fichero al que se redirige la salida estándar de errores.
#PBS -e ptop.err
# Indica que un trabajo no debe volver a ejecutarse si falla.
#PBS -r n
# Exporta todas las variables de entorno para el trabajo.
#PBS -V
# ##############################################################################
# Posicionamiento en el directorio desde el que se ha realizado el qsub del script
cd $PBS_O_WORKDIR
# Obtener un fichero con la información de los nodos y procesadores que se nos
# ha asignado para la ejecución de nuestro trabajo
cat $PBS_NODEFILE > nodesinfo.txt
# Código correspondiente a la ejecución de nuestro programa en particular
mpirun -np 2 ./ptop