function generateNodes(n) {
    return {
        "nodes": Array.from({
            length: n
        }, (_, i) => ({
            "id": i,
            "group": 1
        })),
        "links": []
    }
}

//Funcion chart extraida y adaptada de https://observablehq.com/@bstaats/matrix-diagram#chart
function _chart(n) {
    data = generateNodes(n);
    var margin = {
        top: 50,
        right: 0,
        bottom: 0,
        left: 60
    };
    var width = 1500 - margin.left - margin.right;
    var height = 1500 - margin.top - margin.bottom;
    //const color = d3.scaleOrdinal(d3.schemeCategory10).domain(d3.range(10));
    var opacity = d3.scaleLinear()
        .domain([0, 4])
        .range([0.25, 1])
        .clamp(true);
    var x = d3.scaleBand()
        .rangeRound([0, width])
        .paddingInner(0.1)
        .align(0);

    var svgDOM = d3.select(document.createElementNS("http://www.w3.org/2000/svg", "svg"))
        .attr('class', 'matrixdiagram')
        .attr('width', width + margin.left + margin.right)
        .attr('height', height + margin.top + margin.bottom);

    var svg = svgDOM.append('g')
        .attr('transform', 'translate(' + margin.left + ',' + margin.top + ')');

    var graph = data;

    var idToNode = {};
    graph.nodes.forEach(function(n) {
        n.degree = 0;
        idToNode[n.id] = n;
    });

    graph.nodes.sort(function(a, b) {
        return b.group - a.group;
    });

    x.domain(d3.range(graph.nodes.length));
    opacity.domain([0, d3.max(graph.nodes, function(d) {
        return d.degree;
    })]);

    var matrix = graph.nodes.map(function(outer, i) {
        outer.index = i;
        return graph.nodes.map(function(inner, j) {
            return {
                i: i,
                j: j,
                val: i === j ? inner.degree : 0
            };
        });
    });
    graph.links.forEach(function(l) {
        matrix[l.source.index][l.target.index].val += l.value;
        matrix[l.target.index][l.target.index].val += l.value;
        matrix[l.source.index][l.source.index].val += l.value;
        matrix[l.target.index][l.source.index].val += l.value;
    });
    var row = svg.selectAll('g.row')
        .data(matrix)
        .enter().append('g')
        .attr('class', 'row')
        .attr('transform', function(d, i) {
            return 'translate(0,' + x(i) + ')';
        })
        .each(makeRow, function(d, i) {
            return i;
        });
    row.append('text')
        .attr('id', function(d, i) {
            return 'row-' + i;
        })
        .attr('class', 'label')
        .attr('x', -4)
        .attr('y', x.bandwidth() / 2)
        .attr('dy', '0.32em')
        .text(function(d, i) {
            return graph.nodes[i].id;
        });
    var column = svg.selectAll('g.column')
        .data(matrix)
        .enter().append('g')
        .attr('id', function(d, i) {
            return 'col-' + i;
        })
        .attr('class', 'column')
        .attr('transform', function(d, i) {
            return 'translate(' + x(i) + ', 0)rotate(-90)';
        })
        .append('text')
        .attr('class', 'label')
        .attr('x', 4)
        .attr('y', x.bandwidth() / 2)
        .attr('dy', '0.32em')
        .text(function(d, i) {
            return graph.nodes[i].id;
        });

    function makeRow(rowData, row) {
        var cell = d3.select(this).selectAll('rect')
            .data(rowData)
            .enter().append('rect')
            // .attr('class', 'cell')
            .attr('id', function(d, i) {
                return row + "," + i;
            })
            .attr('x', function(d, i) {
                return x(i);
            })
            .attr('width', x.bandwidth())
            .attr('height', x.bandwidth())

        cell.append('title')
            .text(function(d, i) {
                return "( " + row + " , " + i + " )";
            })

    }
    document.getElementById("matrix").innerHTML="";
    document.getElementById("matrix").appendChild(svgDOM.node());
    return true
}

interval = undefined
queueToChange = []
intervalOn =  1
delay = 20
//Cada n milisengundos revisa si tiene que cambiar un nodo de estado y hace el primero

function changeDelay(_delay){
    delay=_delay
    clearInterval(interval)
    interval = setInterval(checkPending, delay)
}
function changeEstado(id,estado){
    document.getElementById(id).removeAttribute('class')
    //document.getElementById(id).className = ""
    document.getElementById(id).classList.add(estado)
}
function checkPending(){
    if(queueToChange.length){
        changeEstado(queueToChange[0][0],queueToChange[0][1])
        queueToChange.shift()
    }
    else {
        intervalOn = 0
        clearInterval(interval)
    }
}

function changeNow(node, estado){
    /*default visitado camino actual resultado inicio fin none*/
    changeEstado(node[1][0]+","+node[1][1],estado)
}

function addChange(node, estado){
    /*default visitado camino actual resultado inicio fin none*/
    queueToChange.push([node[1][0]+","+node[1][1],estado])
    if(!intervalOn){
        interval = setInterval(checkPending, delay)
        intervalOn = 1
    }
}

function mostrarCamino(recorrido){//array de nodos
    recorrido.forEach(node =>{
        addChange(node, "resultado")
    })
}
function restablecerNodosExistentes(){
    clearInterval(interval)
    queueToChange = []
    intervalOn = 0
    for(let i=0;i<graph.tam_grafo;i++){
        coord=graph.getCoordenada(i)
        document.getElementById(coord[0]+","+coord[1]).removeAttribute('class')
        if(graph.grafo[i]==null){
            document.getElementById(coord[0]+","+coord[1]).classList.add('none')
        }
    }

}

interval = setInterval(checkPending, delay)

