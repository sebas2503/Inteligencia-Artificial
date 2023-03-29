// create a graph class
class Graph {
  // defining vertex array and
  // adjacent vecinos
  constructor(vertices)
  {
      this.tam_grafo = vertices;
      this.grafo = Array(vertices);
      for(let i=0;i<this.grafo.length;i++)
      {
        this.grafo[i]=[];
      }
  }

  getCoordenada(index,n=Math.sqrt(this.tam_grafo,2)){ //(198,100)
    let y = index%n; // y = 198%100 = 98
    return [(index-y)/n, y]; // 1,98
  }

  getDistancia(punto1, punto2)
  {
    return Math.sqrt(Math.pow(punto1[0]-punto2[0],2)+Math.pow(punto1[1]-punto2[1],2)); 
  }

  duplicado(vecinos,nodo){ // duplicado([2]-> [11,3.5],[11,3.5])
    for(let j=0; j< vecinos.length;j++){ // recorre de 0 a 1
      if(vecinos[j][0]==nodo[0]&& vecinos[j][1]== nodo[1]) // si un nodo ya se encuentra en la lista de adyacencia
      {
        //console.log(vecinos[j]);
        return true;
      }
        //return true;
    }
    return false;

}
  // functions to be implemented
  addEdge(inicio,final)
  {
    let coordenada_inicio=this.getCoordenada(inicio);
    let coordenada_final=this.getCoordenada(final);
    let distancia = this.getDistancia(coordenada_inicio,coordenada_final);

    if(this.grafo[inicio].length==0) // si estÃ¡ vacio, hay que conectarlo
    {
      this.grafo[inicio].push([final,distancia]);
    }

    if(this.grafo[final].length==0) // si estÃ¡ vacio, hay que conectarlo
    {
      this.grafo[final].push([inicio,distancia]);
    }

    // al ya haber un nodo, debe terminar de conectarse con sus otros nodos cercanos, pero sin repetir los que ya tiene.
    if(this.duplicado(this.grafo[inicio],[final,distancia])== false){ // duplicado([2],[11,3.5])
      this.grafo[inicio].push([final,distancia]);
    }
    if(this.duplicado(this.grafo[final],[inicio,distancia])== false){
      this.grafo[final].push([inicio,distancia]);
    }

  }
  
  print() {
    for(let i=0;i<this.grafo.length;i++){
        console.log("vertice",i);
        console.log("numero de aristas",this.grafo[i].length);
        for(let j= 0; j<this.grafo[i].length;j++){
            console.log("arista",j,this.grafo[i][j])
        }
    }            
}
  
  removeEdge(inicio, final){ // si queremos eliminar el edge del nodo 2 y 101
    if(this.grafo[inicio]) { // this.grafo[2][[1,distancia],[101,d],[102,d]...]
        for( let i=0;i<this.grafo[inicio].length;){
            if(this.grafo[inicio][i][0] == final){
                //Elimina el elemento poniendo en su lugar el ultimo elemento y eliminando el duplicado
                this.grafo[inicio][i]=this.grafo[inicio][this.grafo[inicio].length-1] // this.grafo[2][[1,distancia],[102,d],[102,d]...]
                this.grafo[inicio].pop() // pop elimina el ultimo elemento del array this.grafo[2][[1,distancia],[102,d]...]
            }
            i++
        }
    }
    //Repite el procedimiento para el edge en el nodo final
    if(this.grafo[final]) {
        for( let i=0;i<this.grafo[final].length;){
            if(this.grafo[final][i][0] == inicio){
                this.grafo[final][i]=this.grafo[final][this.grafo[final].length-1]
                this.grafo[final].pop()
            }
            i++
        }
        
    }
  }

  removeNode(nodo)
  {
    // se copia todo el array del nodo que se va a borrar para saber con quienes estÃ¡ conectado.
        let edges = [...this.grafo[nodo]];
        for(let i=0;i<edges.length;i++)
        {
          this.removeEdge(nodo,edges[i][0]); // entro a arista[0] porque ese es el nodo, arista[1] es la distancia
        }
        
        //Elimina el array de vertices indicando que el nodo no existe
        this.grafo[nodo] = null;
        addChange([nodo,this.getCoordenada(nodo)],"none" )
  }

  removeNodes(porcentaje){//0.2
    let c=0
      let indices = Array.from(Array(this.tam_grafo).keys()) // crea un array de numeros secuenciales de 0 a tam_grafo [0 ... 9999]
      indices.sort(() => Math.random() - 0.5); // Funcion aleatoria
      let nodosABorrar = indices.slice(0,porcentaje*this.tam_grafo); // se crea un array que solo tenga los primeros n numeros a eliminar

      for(let i=0;i<nodosABorrar.length;i++)
      {
        this.removeNode(nodosABorrar[i]);
      }
      
  }

  createGraph()
  {
        let size_lado=Math.sqrt(this.tam_grafo,2);
        ///////////////////////////////////////////////////////////
        //create corners
        let esquinas=[0,size_lado-1,size_lado*(size_lado-1),size_lado*(size_lado-1)+(size_lado-1)];// [0,99,9900,9999]
        
        //0 && 9999
        let vecinos_lurd=[size_lado+1,1,size_lado]/// [101,100,1] left up right down
        for(let i=0;i<3;i++){
            this.addEdge(esquinas[0], esquinas[0] + vecinos_lurd[i]); // 0, 1,100,101
            this.addEdge(esquinas[3], esquinas[3] - vecinos_lurd[i]); // 9999, 9898,9899,9998
        }
        //99 && 9900
        let vecinos_ldru=[(size_lado-1),-1,size_lado]/// [99,-1,100] left down right up
        for(let i=0;i<3;i++){
            this.addEdge(esquinas[1], esquinas[1] + vecinos_ldru[i]);
            this.addEdge(esquinas[2], esquinas[2] - vecinos_ldru[i]);
        }
        ////////////////////////////////////////////////////////////
  
        //////////////////////////////////////////////////////////
        //create side left
        let linea_izquierda=[-1*(size_lado-1),size_lado+1,1,size_lado,-size_lado] ///[-99,101,1,100,-100] diagonales, derecha, abajo, arriba
        for(let i=size_lado;i<size_lado*(size_lado-1);i=i+size_lado){
          this.addEdge(i, i + linea_izquierda[0]);
          this.addEdge(i, i + linea_izquierda[1]);
          this.addEdge(i, i + linea_izquierda[2]);
          this.addEdge(i, i + linea_izquierda[3]);
          this.addEdge(i, i + linea_izquierda[4]);
        }
        ////////////////////////////////////////////////////////////
        //create side right
        let linea_derecha=[-1*(size_lado+1),size_lado-1,-1,size_lado,-size_lado];///[-101,99,-1,100,-100]
        for(let i=size_lado+size_lado-1;i<size_lado*(size_lado-1)+(size_lado-1);i=i+size_lado){
          this.addEdge(i, i + linea_derecha[0]);
          this.addEdge(i, i + linea_derecha[1]);
          this.addEdge(i, i + linea_derecha[2]);
          this.addEdge(i, i + linea_derecha[3]);
          this.addEdge(i, i + linea_derecha[4]);
        }
  
        ////////////////////////////////////////////////////////////
        //create side up
        let linea_arriba=[size_lado+1,size_lado-1,-1,1,size_lado];///[101,99,-1,1,100]
        for(let i=1;i<size_lado-1;i++){
          this.addEdge(i, i + linea_arriba[0]);
          this.addEdge(i, i + linea_arriba[1]);
          this.addEdge(i, i + linea_arriba[2]);
          this.addEdge(i, i + linea_arriba[3]);
          this.addEdge(i, i + linea_arriba[4]);
        }
  
        ////////////////////////////////////////////////////////////
        //create side down
        let linea_abajo=[-1*(size_lado-1),-1*(size_lado+1),-1,1,-size_lado];///[-99,-101,-1,1,-100]
        for(let i=size_lado*(size_lado-1)+1;i<size_lado*(size_lado-1)+(size_lado-1);i++){
          this.addEdge(i, i + linea_abajo[0]);
          this.addEdge(i, i + linea_abajo[1]);
          this.addEdge(i, i + linea_abajo[2]);
          this.addEdge(i, i + linea_abajo[3]);
          this.addEdge(i, i + linea_abajo[4]);
        }
        ////////////////////////////////////////////////////////////
        //create centers
        let vecinos_centro=[size_lado-1,-1*(size_lado-1),size_lado+1,-1*(size_lado+1),-1,1,size_lado,-size_lado];///[99,-99,101,-101,-1,1,100,-100]
        for(let i=size_lado+1;i<size_lado*(size_lado-1)+1;i=i+size_lado){
          for(let j=i;j<i+size_lado-2;j++){
            this.addEdge(j, j + vecinos_centro[0]);
            this.addEdge(j, j + vecinos_centro[1]);
            this.addEdge(j, j + vecinos_centro[2]);
            this.addEdge(j, j + vecinos_centro[3]);
            this.addEdge(j, j + vecinos_centro[4]);
            this.addEdge(j, j + vecinos_centro[5]);
            this.addEdge(j, j + vecinos_centro[6]);
            this.addEdge(j, j + vecinos_centro[7]);
          }
  
        }
  }

  destructor()
  {
    
    this.grafo = Array(this.tam_grafo);
    for(let i=0;i< this.tam_grafo;i++){
      this.grafo[i]=[];
    }
  
  }


  dfs_recursion(nodo, destino, visitados,recorrido){
    try{addChange(nodo,"actual")}catch (error){}
    
    if(nodo[0]==destino[0]){
      recorrido.push(nodo)
        return true
    }
    let edges = this.grafo[nodo[0]]
    //arista [nodo, distancia]
    for(let i=0;i<edges.length;i++){
        if(!visitados[edges[i][0]]){
            visitados[edges[i][0]]=1
              recorrido.push(nodo);
            
            if(this.dfs_recursion([edges[i][0],this.getCoordenada(edges[i][0])], destino, visitados,recorrido)) return true;
            recorrido.pop()
        }
    }
    return false;
  }

  dfs(origen,destino){
    let visitados = Array(this.tam_grafo).fill(0);//Para guardar que nodos ya visite
    visitados[origen[0]]=1
    //console.log(origen[0]);
    let recorrido = []
    let encontro = this.dfs_recursion(origen,destino,visitados,recorrido)
    mostrarCamino(recorrido)
    return encontro
  }


  

  bfs(origen,destino){
    let recorrido = [];
    let visitados = Array(this.tam_grafo).fill(0);
    visitados[origen[0]]=1; // el primer elemento se coloca como visitado
    var queue=[]
    queue.push(origen); // se aÃ±ade el primer nodo a la cola
    while(queue.length){ // mientras que la cola no este vacia
        let NodoActual=queue[0];
        let nodo = [NodoActual[0],this.getCoordenada(NodoActual[0])];
        try{addChange(nodo,"actual")}catch (error){}//pinto recorrido
        recorrido.push(nodo);
        
        if(NodoActual[0]==destino[0]) { break;}
        queue.shift();// extraigo el primer elemento de la cola
        for(let i = 0; i<this.grafo[NodoActual[0]].length;i++){
            if(visitados[this.grafo[NodoActual[0]][i][0]]==0){
                visitados[this.grafo[NodoActual[0]][i][0]]=1; // si no fue visitado, lo visito
                queue.push(this.grafo[NodoActual[0]][i]);// y lo aÃ±ado al final de la cola
            }
        }
        
        //addChange(origen,"inicio");
        //addChange(destino,"fin");
      }
      mostrarCamino(recorrido); 
  }

}
