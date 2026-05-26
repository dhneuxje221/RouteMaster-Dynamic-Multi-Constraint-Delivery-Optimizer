
class GeneticAlgorithm {

    constructor(locations, depot){
        this.locations = locations;
        this.depot = depot;
        this.populationSize = 60;
    }

    groupByPriority(route){
        const vip = route.filter(o => o.priority === 3);
        const medium = route.filter(o => o.priority === 2);
        const low = route.filter(o => o.priority === 1);

        return [...vip, ...medium, ...low];
    }

    generatePopulation(){
        let population = [];

        const vip = this.locations.filter(o => o.priority === 3);
        const medium = this.locations.filter(o => o.priority === 2);
        const low = this.locations.filter(o => o.priority === 1);

        for(let i=0;i<this.populationSize;i++){
            let route = [
                ...vip.sort(()=>Math.random()-0.5),
                ...medium.sort(()=>Math.random()-0.5),
                ...low.sort(()=>Math.random()-0.5)
            ];

            population.push(route);
        }

        return population;
    }

    routeDistance(route){
        let distance = 0;
        let current = this.depot;

        for(let stop of route){
            distance += haversineDistance(
                current.lat,
                current.lng,
                stop.lat,
                stop.lng
            );

            current = stop;
        }

        return distance;
    }

    priorityPenalty(route){
        let penalty = 0;

        for(let i=0;i<route.length;i++){
            if(route[i].priority === 3){
                penalty += i * 15;
            }
            else if(route[i].priority === 2){
                penalty += i * 5;
            }
        }

        return penalty;
    }

    deadlinePenalty(route){
        let penalty = 0;
        let currentMinutes = 9 * 60;

        for(let stop of route){
            currentMinutes += 20;

            const [h,m] = stop.deadline.split(':').map(Number);
            const deadlineMinutes = h * 60 + m;

            if(currentMinutes > deadlineMinutes){
                penalty += (currentMinutes - deadlineMinutes) * 2;
            }
        }

        return penalty;
    }

    fitness(route){
        let distance = this.routeDistance(route);
        let priorityPenalty = this.priorityPenalty(route);
        let deadlinePenalty = this.deadlinePenalty(route);

        return 1 / (distance + priorityPenalty + deadlinePenalty + 1);
    }

    bestRoute(){
        let population = this.generatePopulation();
        let best = population[0];

        for(let route of population){
            route = this.groupByPriority(route);

            if(this.fitness(route) > this.fitness(best)){
                best = route;
            }
        }

        return this.groupByPriority(best);
    }
}
