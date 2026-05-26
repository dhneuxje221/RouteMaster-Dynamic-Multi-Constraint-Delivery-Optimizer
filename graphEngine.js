
class GraphEngine {

    constructor(){
        this.graph = {};
    }

    addVertex(v){
        if(!this.graph[v]){
            this.graph[v] = [];
        }
    }

    addEdge(source,destination,weight){
        this.addVertex(source);
        this.addVertex(destination);

        this.graph[source].push({
            node: destination,
            weight: weight
        });
    }

    dijkstra(start){
        let distances = {};
        let visited = {};
        let pq = [];

        for(let vertex in this.graph){
            distances[vertex] = Infinity;
        }

        distances[start] = 0;
        pq.push({ node:start, distance:0 });

        while(pq.length){
            pq.sort((a,b)=>a.distance-b.distance);
            let current = pq.shift();

            if(visited[current.node]) continue;
            visited[current.node] = true;

            for(let neighbor of this.graph[current.node]){
                let distance = distances[current.node] + neighbor.weight;

                if(distance < distances[neighbor.node]){
                    distances[neighbor.node] = distance;
                    pq.push({ node:neighbor.node, distance });
                }
            }
        }

        return distances;
    }
}
