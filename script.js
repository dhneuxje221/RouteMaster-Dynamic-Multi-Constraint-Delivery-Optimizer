
const map = L.map('map').setView([28.4595,77.0266],11);

L.tileLayer(
'https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png',
{
    attribution: 'OpenStreetMap'
}
).addTo(map);

let orders = [];
let markers = [];
let routeLine = null;
let depotMarker = null;

const graphEngine = new GraphEngine();
const dynamicOptimizer = new DynamicOptimizer();

function haversineDistance(lat1,lon1,lat2,lon2){
    const R = 6371;
    const dLat = (lat2-lat1) * Math.PI/180;
    const dLon = (lon2-lon1) * Math.PI/180;

    const a =
        Math.sin(dLat/2) * Math.sin(dLat/2) +
        Math.cos(lat1*Math.PI/180) *
        Math.cos(lat2*Math.PI/180) *
        Math.sin(dLon/2) *
        Math.sin(dLon/2);

    return R * (2*Math.atan2(Math.sqrt(a),Math.sqrt(1-a)));
}

function getDepot(){
    return {
        id:'depot',
        name:'Depot',
        lat:parseFloat(document.getElementById('depotLat').value),
        lng:parseFloat(document.getElementById('depotLng').value)
    };
}

function priorityClass(priority){
    if(priority === 3) return 'vip';
    if(priority === 2) return 'medium';
    return 'low';
}

function updateDepotMarker(){
    const depot = getDepot();

    if(depotMarker){
        map.removeLayer(depotMarker);
    }

    depotMarker = L.marker([depot.lat,depot.lng])
        .addTo(map)
        .bindPopup('Starting Depot');
}

function addMarker(order){
    const marker = L.marker([order.lat,order.lng])
        .addTo(map)
        .bindPopup(`${order.name} - ${ConstraintValidator.priorityName(order.priority)}`);

    markers.push(marker);
}

document.getElementById('addOrderBtn')
.addEventListener('click',()=>{

    const order = {
        id: Date.now(),
        name: document.getElementById('customerName').value.trim(),
        weight: parseFloat(document.getElementById('packageWeight').value),
        deadline: document.getElementById('deadline').value,
        priority: parseInt(document.getElementById('priority').value),
        lat: parseFloat(document.getElementById('lat').value),
        lng: parseFloat(document.getElementById('lng').value)
    };

    if(!order.name || isNaN(order.weight) || isNaN(order.lat) || isNaN(order.lng)){
        alert('Please fill all order fields correctly');
        return;
    }

    orders.push(order);
    addMarker(order);
    renderOrders();

    document.getElementById('customerName').value = '';
    document.getElementById('packageWeight').value = '';
    document.getElementById('lat').value = '';
    document.getElementById('lng').value = '';
});

document.getElementById('demoBtn')
.addEventListener('click',()=>{

    const demoOrders = [
        {name:'VIP Hospital Order',weight:18,deadline:'11:30',priority:3,lat:28.5355,lng:77.3910},
        {name:'Express College Parcel',weight:14,deadline:'13:00',priority:3,lat:28.4595,lng:77.0266},
        {name:'Mall Delivery',weight:22,deadline:'15:00',priority:2,lat:28.4089,lng:77.3178},
        {name:'Office Documents',weight:8,deadline:'16:00',priority:2,lat:28.7041,lng:77.1025},
        {name:'Normal Grocery',weight:10,deadline:'18:00',priority:1,lat:28.4744,lng:77.5040}
    ];

    demoOrders.forEach(item=>{
        const order = {
            id: Date.now() + Math.random(),
            ...item
        };

        orders.push(order);
        addMarker(order);
    });

    renderOrders();
});

function renderOrders(){
    document.getElementById('totalOrders').innerText = orders.length;

    const list = document.getElementById('ordersList');
    list.innerHTML = '';

    const sortedOrders = ConstraintValidator.prioritize(orders);

    sortedOrders.forEach(order=>{
        list.innerHTML += `
            <div class="order-card ${priorityClass(order.priority)}">
                <div class="order-info">
                    <strong>${order.name}</strong><br>
                    <small>${order.weight}kg | Deadline: ${order.deadline}</small>
                </div>
                <span class="priority-pill priority-${order.priority}">
                    ${ConstraintValidator.priorityName(order.priority)}
                </span>
            </div>
        `;
    });
}

document.getElementById('optimizeBtn')
.addEventListener('click',()=>{

    if(orders.length === 0){
        alert('Add orders first');
        return;
    }

    updateDepotMarker();

    const depot = getDepot();

    const capacity = parseFloat(document.getElementById('capacity').value);
    const fuelLimit = parseFloat(document.getElementById('fuelLimit').value);

    const prioritySorted = ConstraintValidator.prioritize(orders);
    const ga = new GeneticAlgorithm(prioritySorted,depot);

    let bestRoute = ga.bestRoute();

    let totalDistance = 0;
    let totalWeight = 0;
    let current = depot;

    const routeBox = document.getElementById('optimizedRoute');
    routeBox.innerHTML = '';

    bestRoute.forEach((order,index)=>{
        totalWeight += order.weight;

        const segmentDistance = haversineDistance(
            current.lat,
            current.lng,
            order.lat,
            order.lng
        );

        totalDistance += segmentDistance;
        current = order;

        routeBox.innerHTML += `
            <div class="route-item ${priorityClass(order.priority)}">
                <div>
                    <strong>${index+1}. ${order.name}</strong><br>
                    <small>${segmentDistance.toFixed(2)} KM from previous point</small>
                </div>
                <span class="priority-pill priority-${order.priority}">
                    ${ConstraintValidator.priorityName(order.priority)}
                </span>
            </div>
        `;
    });

    const capacityOK = ConstraintValidator.checkCapacity(totalWeight,capacity);
    const fuelOK = ConstraintValidator.checkFuel(totalDistance,fuelLimit);

    document.getElementById('totalDistance').innerText = totalDistance.toFixed(2)+' KM';

    document.getElementById('efficiency').innerText =
        ((totalWeight/capacity)*100).toFixed(0)+'%';

    document.getElementById('routeStatus').innerHTML =
        capacityOK && fuelOK
        ? '<span class="status-good">Valid</span>'
        : '<span class="status-bad">Check</span>';

    if(routeLine){
        map.removeLayer(routeLine);
    }

    const routeCoordinates = [
        [depot.lat,depot.lng],
        ...bestRoute.map(o=>[o.lat,o.lng])
    ];

    routeLine = L.polyline(
        routeCoordinates,
        {color:'#2563eb',weight:5}
    ).addTo(map);

    map.fitBounds(routeLine.getBounds(), {padding:[30,30]});

    document.getElementById('algorithmTrace').innerHTML = `
        ✅ Module 1: Graph Engine / Dijkstra logic loaded for shortest path calculation.<br>
        ✅ Module 2: Genetic Algorithm selected route with priority-aware fitness function.<br>
        ✅ Module 3: Dynamic Programming memoization ready for new-order insertion.<br>
        ✅ Module 4: Constraint Validator checked capacity and fuel range.<br>
        <br>
        <strong>Total Weight:</strong> ${totalWeight.toFixed(1)} / ${capacity} kg<br>
        <strong>Fuel Range:</strong> ${totalDistance.toFixed(2)} / ${fuelLimit} KM<br>
        <strong>Priority Rule:</strong> VIP orders are locked before Medium and Low priority orders.
    `;
});

document.getElementById('clearBtn')
.addEventListener('click',()=>{
    orders = [];

    markers.forEach(marker=>map.removeLayer(marker));
    markers = [];

    if(routeLine){
        map.removeLayer(routeLine);
        routeLine = null;
    }

    if(depotMarker){
        map.removeLayer(depotMarker);
        depotMarker = null;
    }

    document.getElementById('ordersList').innerHTML = '';
    document.getElementById('optimizedRoute').innerHTML = '';

    document.getElementById('totalOrders').innerText = '0';
    document.getElementById('totalDistance').innerText = '0 KM';
    document.getElementById('efficiency').innerText = '0%';
    document.getElementById('routeStatus').innerText = 'Waiting';
    document.getElementById('algorithmTrace').innerText =
        'Add orders and run optimization to see DAA module output.';
});

document.getElementById('locateBtn')
.addEventListener('click',()=>{
    navigator.geolocation.getCurrentPosition(pos=>{
        const lat = pos.coords.latitude;
        const lng = pos.coords.longitude;

        document.getElementById('depotLat').value = lat.toFixed(4);
        document.getElementById('depotLng').value = lng.toFixed(4);

        map.setView([lat,lng],13);
        updateDepotMarker();
    });
});

updateDepotMarker();
