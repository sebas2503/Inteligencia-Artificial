_size = 100
graph = new Graph(_size*_size)
nodoInicio = [0,[0,0]],
nodoFinal = [_size*_size-1,graph.getCoordenada(_size*_size-1)]
_clickStatus = ""
_porcentaje = 20

function addClickListener(){
    for(let i=0;i<_size;i++){
        for( let j=0;j<_size;j++){
            document.getElementById(i+","+j).addEventListener("click",controles.funciones._click )
        }
    }
}

controles = {
    buttons_id : {
        crear_nuevo_grafo: "crear",
        eliminar_pocentaje_grafo: "eliminar_porcentaje",
        bfs : "bfs",
        dfs : "dfs",
        seleccionar_inicio:"seleccionarInicio",
        seleccionar_fin:"seleccionarFin",
        seleccionarBorrar:"borrarNodo"
    },
    inputs_id :{
        porcentaje_para_eliminar : "porcentaje",
    },
    funciones:{
        crear_grafo(){
            graph.destructor()
            graph.createGraph()
            _chart(_size)
            nodoInicio = [0,[0,0]],
            nodoFinal = [_size*_size-1,graph.getCoordenada(_size*_size-1)]
            addChange(nodoInicio,"inicio")
            addChange(nodoFinal,"fin")
            addClickListener()
            document.getElementById(controles.inputs_id.porcentaje_para_eliminar).disabled = false
            document.getElementById("eliminar_porcentaje").disabled=false
        },
        eliminar_nodos(){
            let porcentaje = document.getElementById(controles.inputs_id.porcentaje_para_eliminar)
            graph.removeNodes(_porcentaje/100)
            porcentaje.disabled = true
            this.disabled = true
            restablecerNodosExistentes()
        },
        bfs(){
            restablecerNodosExistentes()
            addChange(nodoInicio,"inicio")
            addChange(nodoFinal,"fin")
            graph.bfs(nodoInicio,nodoFinal)
            addChange(nodoInicio,"inicio")
            addChange(nodoFinal,"fin")
        },
        dfs(){
            restablecerNodosExistentes()
            addChange(nodoInicio,"inicio")
            addChange(nodoFinal,"fin")
            graph.dfs(nodoInicio,nodoFinal)
            addChange(nodoInicio,"inicio")
            addChange(nodoFinal,"fin")
        },
        _click(clickInfo){
            let coord= this.id.split(',')
            coord[0]=Number(coord[0])
            coord[1]=Number(coord[1])
            if(_clickStatus == "seleccionando_inicio"){
                addChange(nodoInicio,"default")
                nodoInicio = [_size*coord[0]+coord[1],coord]
                _clickStatus=""
                addChange(nodoInicio,"inicio")
            }
            else if(_clickStatus == "seleccionando_final"){
                addChange(nodoFinal,"default")
                nodoFinal = [_size*coord[0]+coord[1],coord]
                _clickStatus=""
                addChange(nodoFinal,"fin")
            }
            else if(_clickStatus == "eliminando_nodos"){
                //console.log("Estamos eliminando el nodo ",_size*coord[0]+coord[1])
                graph.removeNode(_size*coord[0]+coord[1])
            }
        },
        porcentaje_update(){
            _porcentaje = this.value
        },

    }
}

//Funciones grafo
document.getElementById(controles.buttons_id.crear_nuevo_grafo).addEventListener("click", controles.funciones.crear_grafo)
document.getElementById(controles.buttons_id.eliminar_pocentaje_grafo).addEventListener("click", controles.funciones.eliminar_nodos)

document.getElementById(controles.inputs_id.porcentaje_para_eliminar).addEventListener("input", controles.funciones.porcentaje_update)

//Busquedas
document.getElementById(controles.buttons_id.bfs).addEventListener("click", controles.funciones.bfs)
document.getElementById(controles.buttons_id.dfs).addEventListener("click", controles.funciones.dfs)

//Inicio y final
document.getElementById(controles.buttons_id.seleccionar_inicio).addEventListener("click", ()=>{_clickStatus = "seleccionando_inicio"})
document.getElementById(controles.buttons_id.seleccionar_fin).addEventListener("click", ()=>{_clickStatus = "seleccionando_final"})
document.getElementById(controles.buttons_id.seleccionarBorrar).addEventListener("click", ()=>{_clickStatus = "eliminando_nodos"})

